#include "gateway/controller/grpc_controller.h"

#include "gateway/gateway.grpc.pb.h"
#include "gateway/gateway.pb.h"
#include "gateway/service_discovery.h"
#include "robocin/network/zmq_request_reply_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <grpcpp/server_builder.h>
#include <string_view>

namespace gateway {

namespace {

constexpr std::string_view kGatewayLive = "Gateway.Live";
constexpr std::string_view kVisionPubService = "Vision.Publisher";
constexpr std::string_view kVisionRepService = "Vision.Reply";

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using protocols::ui::GatewayService;
using protocols::ui::GetVisionChunkRequest;
using protocols::ui::GetVisionChunkResponse;
using robocin::ZmqRequestSocket;
using robocin::ZmqSubscriberSocket;

class GatewayServiceImpl final : public GatewayService::Service {
 public:
  GatewayServiceImpl() {
    requester_.connect(SServiceDiscovery.lookup(kVisionRepService).address);

    {
      auto [address, topics] = SServiceDiscovery.lookup(kVisionPubService);
      subscriber_.connect(address, topics);
    }
  }

  Status GetVisionChunk(ServerContext* context,
                        const GetVisionChunkRequest* request,
                        GetVisionChunkResponse* response) override {

    requester_.send(std::string_view{"Mock"});
    auto reply = requester_.receive();
    *response->mutable_header()->mutable_request_start() = request->header().start();
    return Status::OK;
  }

 private:
  ZmqRequestSocket requester_;
  ZmqSubscriberSocket subscriber_;
};

} // namespace

GrpcController::GrpcController(std::string_view address) : address_{address} {}

void GrpcController::run() {
  ServerBuilder builder;
  builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
  GatewayServiceImpl service;
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  server->Wait();
}

} // namespace gateway

#include "gateway/controller/grpc_controller.h"

#include "gateway/service_discovery.h"
#include "robocin/network/zmq_request_reply_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <grpcpp/server_builder.h>
#include <protocols/ui/gateway.grpc.pb.h>
#include <protocols/ui/gateway.pb.h>
#include <protocols/ui/replay.pb.h>
#include <string>
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
    requester_.send(serialize(*request));
    auto reply = requester_.receive();
    *response = deserialize(reply);
    return Status::OK;
  }

 private:
  ZmqRequestSocket requester_;
  ZmqSubscriberSocket subscriber_;

  static std::string serialize(const GetVisionChunkRequest& request) {
    std::string bytes;
    request.SerializeToString(&bytes);
    return bytes;
  }

  static GetVisionChunkResponse deserialize(std::string_view reply) {
    GetVisionChunkResponse chunk;
    chunk.ParseFromString(std::string{reply});
    return chunk;
  }
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

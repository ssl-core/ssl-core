#include "gateway/controller/grpc_controller.h"

#include "gateway/service_discovery.h"
#include "robocin/network/zmq_request_reply_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <grpcpp/server_builder.h>
#include <protocols/ui/gateway.grpc.pb.h>
#include <protocols/ui/gateway.pb.h>
#include <protocols/vision/frame.pb.h>
#include <string>
#include <chrono>
#include <format>
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
using protocols::ui::ReceiveLiveStreamRequest;
using protocols::ui::ReceiveLiveStreamResponse;
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
    std::cout << "Receive GetVisionChunk request" << std::endl;
    requester_.send(request->SerializeAsString());
    auto reply = requester_.receive();
    *response = deserialize<GetVisionChunkResponse>(reply);
    return Status::OK;
  }

  Status ReceiveLiveStream(ServerContext* context,
                           const ReceiveLiveStreamRequest* request,
                           grpc::ServerWriter<ReceiveLiveStreamResponse>* writer) override {
    // TODO(aalmds): Poller for specific subscribers.
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
      auto reply = subscriber_.receive();
      if (reply.message.empty()) {
        continue;
      }
      auto end = std::chrono::high_resolution_clock::now();
      auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1e6;
      start = std::move(end);
      std::cout << std::format("sending after {} ms.", diff) << std::endl;
      // std::cout << "ReceiveLiveStream: Replying" << std::endl;
      ReceiveLiveStreamResponse response;
      *response.mutable_payload()->mutable_vision_frame()
          = deserialize<protocols::vision::Frame>(reply.message);
      writer->Write(response);
    }
  }

 private:
  ZmqRequestSocket requester_;
  ZmqSubscriberSocket subscriber_;

  template <class Response>
  static Response deserialize(std::string_view reply) {
    Response chunk;
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

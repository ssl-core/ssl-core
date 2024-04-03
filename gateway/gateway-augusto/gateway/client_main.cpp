#include "gateway/service_discovery.h"

#include <google/protobuf/duration.pb.h>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <protocols/ui/gateway.grpc.pb.h>
#include <protocols/ui/gateway.pb.h>
#include <string>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using protocols::ui::GatewayService;
using protocols::ui::GetVisionChunkRequest;
using protocols::ui::GetVisionChunkResponse;

class GatewayClient {
 public:
  explicit GatewayClient(const std::shared_ptr<Channel>& channel) :
      stub_(GatewayService::NewStub(channel)) {}

  void sendRequest(const google::protobuf::Duration& start_time) {
    GetVisionChunkRequest request;
    *request.mutable_header()->mutable_start() = start_time;

    GetVisionChunkResponse response;
    ClientContext context;

    std::cout << "here\n";
    Status status = stub_->GetVisionChunk(&context, request, &response);

    if (status.ok()) {
      std::cout << response.header().request_start().DebugString();
      return;
    }

    std::cout << "RPC failed: " << status.error_message() << std::endl;
  }

 private:
  std::unique_ptr<GatewayService::Stub> stub_;
};

int main() {
  GatewayClient client(grpc::CreateChannel("gateway:50051", grpc::InsecureChannelCredentials()));

  google::protobuf::Duration duration;
  duration.set_seconds(42);

  client.sendRequest(duration);

  return 0;
}


#include "gateway/controller/grpc_controller.h"
#include "gateway/controller/icontroller.h"
#include "gateway/controller/third_party_sockets_controller.h"
#include "gateway/service_discovery.h"
#include "grpc_controller.h"
#include "singleton.h"

#include <fstream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <protocols/ui/gateway.grpc.pb.h>
#include <protocols/ui/gateway.pb.h>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using gateway::GrpcController;
using gateway::IController;
using gateway::SServiceDiscovery;
using gateway::ThirdPartySocketsController;

static constexpr std::string_view kServiceRegistryJson = "service_registry.json";
constexpr std::string_view kGatewayGrpc = "Gateway.Grpc";

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "first arg must be the inet parameter.";
    return -1;
  }

  std::span args{argv + 1, argv + argc - 1};

  std::cout << "Gateway is runnning!" << std::endl;
  std::cout << std::format("inet is '{}'.", args[0]) << std::endl;

  // saving inet into a singleton of string.
  gateway::Singleton<std::string>::get() = args[0];

  std::ifstream file(std::format("{}/{}", ROBOCIN_REPOSITORY_PATH, kServiceRegistryJson));
  if (!file.is_open()) {
    return -1;
  }

  SServiceDiscovery.setup(nlohmann::json::parse(file));

  std::jthread third_party_sockets_controller_thread([]() {
    std::unique_ptr<IController> third_party_sockets_controller
        = std::make_unique<ThirdPartySocketsController>();

    third_party_sockets_controller->run();
  });

  std::string gateway_grpc_address(SServiceDiscovery.lookup(kGatewayGrpc).address);
  std::unique_ptr<IController> grpc_controller
      = std::make_unique<GrpcController>(gateway_grpc_address);
  grpc_controller->run();

  return 0;
}


#include "gateway/controller/asynchronous_sockets_controller.h"
#include "gateway/controller/icontroller.h"
#include "gateway/controller/synchronous_sockets_controller.h"
#include "gateway/controller/third_party_sockets_controller.h"
#include "gateway/service_discovery.h"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using gateway::AsynchronousSocketsController;
using gateway::IController;
using gateway::SServiceDiscovery;
using gateway::SynchronousSocketsController;
using gateway::ThirdPartySocketsController;

static constexpr std::string_view kServiceDomainJson = "service_domain.json";

int main() {
  std::ifstream file(std::format("{}/{}", ROBOCIN_REPOSITORY_PATH, kServiceDomainJson));
  if (!file.is_open()) {
    return -1;
  }

  SServiceDiscovery.setup(nlohmann::json::parse(file));

  std::jthread third_party_sockets_thread([]() {
    std::unique_ptr<IController> third_party_sockets_controller
        = std::make_unique<ThirdPartySocketsController>();

    third_party_sockets_controller->run();
  });

  std::jthread synchronous_sockets_thread([]() {
    std::unique_ptr<IController> synchronous_sockets_controller
        = std::make_unique<SynchronousSocketsController>();

    synchronous_sockets_controller->run();
  });

  std::jthread asynchronous_sockets_thread([]() {
    std::unique_ptr<IController> asynchronous_sockets_controller
        = std::make_unique<AsynchronousSocketsController>();

    asynchronous_sockets_controller->run();
  });

  return 0;
}

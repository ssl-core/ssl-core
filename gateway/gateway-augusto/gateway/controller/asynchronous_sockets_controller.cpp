#include "gateway/controller/asynchronous_sockets_controller.h"

#include "gateway/service_discovery.h"

namespace gateway {
namespace {

constexpr std::string_view kGatewayLive = "Gateway.Live";
constexpr std::string_view kVisionService = "Vision";

} // namespace

AsynchronousSocketsController::AsynchronousSocketsController() {
  publisher_.bind(SServiceDiscovery.lookup(kGatewayLive).address);

  {
    auto [address, topics] = SServiceDiscovery.lookup(kVisionService);
    subscriber_.connect(address, topics);
  }
}

void AsynchronousSocketsController::run() {
  while (true) {
    if (auto datagram = subscriber_.receive(); !datagram.empty()) {
      publisher_.send(datagram);
    }
  }
}

} // namespace gateway

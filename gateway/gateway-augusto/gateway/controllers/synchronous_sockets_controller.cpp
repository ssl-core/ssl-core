#include "gateway/controllers/synchronous_sockets_controller.h"
#include "gateway/service_discovery.h"

namespace gateway {

namespace {
  constexpr std::string_view kGatewayRouter = "Gateway.Router";
  constexpr std::string_view kGatewaySyncRequests = "Gateway.Sync.Requests";
}

SynchronousSocketsController::SynchronousSocketsController() {
  router_.bind(SServiceDiscovery.lookup(kGatewayRouter).address);
  dealer_.bind(SServiceDiscovery.lookup(kGatewaySyncRequests).address);
}

void SynchronousSocketsController::run() { zmq::proxy(router_.socket(), dealer_.socket()); }

} // namespace gateway

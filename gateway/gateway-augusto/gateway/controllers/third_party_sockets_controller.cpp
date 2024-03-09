#include "gateway/controllers/third_party_sockets_controller.h"
#include "gateway/service_discovery.h"
#include <thread>

namespace gateway {
namespace {

constexpr std::string_view kGatewayThirdParties = "Gateway.Third.Parties";

} // namespace

ThirdPartySocketsController::ThirdPartySocketsController() {
  publisher_.bind(SServiceDiscovery.lookup(kGatewayThirdParties).address);
}

void ThirdPartySocketsController::run() {
  using namespace std::chrono_literals;

  while (true) {
    std::this_thread::sleep_for(16ms);
  }
}

} // namespace gateway

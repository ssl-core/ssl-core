#include "gateway/controller/third_party_sockets_controller.h"

#include "gateway/service_discovery.h"

namespace gateway {
namespace {

using namespace std::chrono_literals;

struct UdpArgs {
  std::string_view address;
  std::string_view inet;
  int port{};
};

constexpr std::string_view kGatewayThirdParties = "Gateway.Third.Parties";
constexpr UdpArgs kVisionThirdParty = {
    .address = "224.5.23.2",
    .inet = "192.168.1.19",
    .port = 10020,
};
// constexpr UdpArgs kRefereeThirdParty = {.address = ..., .inet = ..., .port = ...,};
// constexpr UdpArgs kTrackedThirdParty = {.address = ..., .inet = ..., .port = ...,};

} // namespace

ThirdPartySocketsController::ThirdPartySocketsController() {
  vision_.connect(kVisionThirdParty.address, kVisionThirdParty.inet, kVisionThirdParty.port);
  // vision_.connect(kRefereeThirdParty.address, kRefereeThirdParty.inet, kRefereeThirdParty.port);
  // vision_.connect(kTrackedThirdParty.address, kTrackedThirdParty.inet, kTrackedThirdParty.port);

  poller_.push(vision_.fd());
  // poller.push(referee_.fd());
  // poller.push(tracked_.fd());

  publisher_.bind(SServiceDiscovery.lookup(kGatewayThirdParties).address);
}

void ThirdPartySocketsController::run() {
  while (true) {
    poller_.poll(/*timeout=*/-1);

    if (auto vision_message = poller_.recvFrom(vision_); !vision_message.empty()) {
      publisher_.send("vision-third-party", vision_message);
    }
  }
}

} // namespace gateway

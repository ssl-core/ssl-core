#include "gateway/controller/third_party_sockets_controller.h"

#include "gateway/service_discovery.h"

#include <iostream>

namespace gateway {
namespace {

using namespace std::chrono_literals;

struct UdpArgs {
  std::string_view address;
  int port{};
};

constexpr std::string_view kGatewayThirdParties = "Gateway.Third.Parties";
constexpr UdpArgs kVisionThirdParty = {
    .address = "224.5.23.2",
    .port = 10020,
};
// constexpr UdpArgs kRefereeThirdParty = {.address = ..., .port = ...,};
constexpr UdpArgs kTrackedThirdParty = {
    .address = "224.5.23.2",
    .port = 10010,
};

} // namespace

ThirdPartySocketsController::ThirdPartySocketsController() {
  vision_.connect(kVisionThirdParty.address, kVisionThirdParty.port);
  tracked_.connect(kTrackedThirdParty.address, kTrackedThirdParty.port);
  // referee_.connect(kRefereeThirdParty.address, kRefereeThirdParty.port);

  poller_.push(vision_.fd());
  poller_.push(tracked_.fd());
  // poller.push(referee_.fd());

  publisher_.bind(SServiceDiscovery.lookup(kGatewayThirdParties).address);
}

void ThirdPartySocketsController::run() {
  std::cout << "Third party sockets controller is running" << std::endl;
  while (true) {
    poller_.poll(/*timeout=*/-1);

    if (auto vision_message = poller_.recvFrom(vision_); !vision_message.empty()) {
      std::cout << "Sending vision message to publisher" << std::endl;
      publisher_.send({"vision-third-party", vision_message});
    }

    if (auto tracked_message = poller_.recvFrom(tracked_); !tracked_message.empty()) {
      std::cout << "Sending tracked message to publisher" << std::endl;
      publisher_.send({"tracked-third-party", tracked_message});
    }
  }
}

} // namespace gateway

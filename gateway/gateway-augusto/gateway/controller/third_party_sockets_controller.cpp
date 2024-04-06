#include "gateway/controller/third_party_sockets_controller.h"

#include "gateway/service_discovery.h"

#include <iostream>

namespace gateway {
namespace {

using namespace std::chrono_literals;

struct UdpArgs {
  std::string_view address;
  std::string_view inet;
  int port{};
};

constexpr std::string_view kGatewayThirdParties = "Gateway.Publisher.Third.Parties";
constexpr std::string_view kInet = "192.168.18.5";

constexpr UdpArgs kGrSim = {
    .address = "224.5.23.2",
    .inet = kInet,
    .port = 10006,
};

constexpr UdpArgs kSimulatorCLI = {
    .address = "224.5.23.2",
    .inet = kInet,
    .port = 10020,
};

constexpr UdpArgs kVisionThirdParty = kSimulatorCLI;
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
  std::cout << "ThirdPartySocketsController" << std::endl;
  while (true) {
    static constexpr int k_timeout_ms = 10;
    poller_.poll(/*timeout=*/k_timeout_ms);

    if (auto vision_message = poller_.recvFrom(vision_); !vision_message.empty()) {
      std::cout << "Sending" << std::endl;
      publisher_.send("vision-third-party", vision_message);
    }
  }
}

} // namespace gateway

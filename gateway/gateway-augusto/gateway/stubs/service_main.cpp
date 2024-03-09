#include "gateway/service_discovery.h"
#include "gateway/network/zmq_router_dealer_socket.h"
#include "robocin/network/zmq_publisher_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"
#include "service_discovery.h"

#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using gateway::ZmqDealerSocket;
using robocin::ZmqPublisherSocket;
using robocin::ZmqSubscriberSocket;

using namespace std::chrono_literals;

constexpr auto kSleep = 2s;
constexpr std::string_view kGatewayThirdParties = "ipc:///tmp/gateway-async-backend.ipc";
constexpr std::string_view kGatewaySyncRequests = "ipc:///tmp/gateway-sync-backend.ipc";
constexpr std::string_view kVisionService = "ipc:///tmp/vision.ipc";

// class ZmqConsumer {
//  public:
//   ZmqConsumer() {
//     std::string_view topic = "udp";
//     subscriber_.connect("ipc:///tmp/gateway-pub-sync.ipc", std::span{&topic, 1});
//     dealer_.connect("ipc:///tmp/gateway-dealer.ipc");
//   }

//   auto receive() { return dealer_.receive(); }

//  private:
//   ZmqSubscriberSocket subscriber_;
//   ZmqDealerSocket dealer_;
// };

void messageConsumer() {
  ZmqDealerSocket dealer;
  dealer.connect(kGatewaySyncRequests);

  while (true) {
    auto message = dealer.receive();
    dealer.send(message);
  }
}

void messageProducer() {
  ZmqPublisherSocket pub;
  pub.bind(kVisionService);
  while (true) {
    pub.send("frame", "testing-communication");

    // std::this_thread::sleep_for(kSleep);
  }
}

int main() {
  std::jthread consumer(messageConsumer);
  std::jthread producer(messageProducer);
}

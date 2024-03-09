#include "gateway/service_discovery.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;
using robocin::ZmqSubscriberSocket;

constexpr int kRequest = 10;
constexpr auto kSleep = 2s;
constexpr std::string_view kGatewayLive = "ipc:///tmp/gateway-async-frontend.ipc";
constexpr std::string_view kGatewayRouter = "ipc:///tmp/gateway-sync-frontend.ipc";

void syncCommunication() {
  constexpr std::string_view kMessage = "Hello from client!";

  zmq::context_t context(1);
  zmq::socket_t req(context, ZMQ_REQ);
  req.connect(std::string{kGatewayRouter});

  for (int i = 0; i < kRequest; ++i) {
    auto bytes = req.send(zmq::message_t{kMessage}, zmq::send_flags::none);

    zmq::message_t reply;
    auto _ = req.recv(reply, zmq::recv_flags::none);
    std::cout << "SYNC: reply " << reply.to_string() << "\n";

    std::this_thread::sleep_for(kSleep);
  }
}

void asyncCommunication() {
  ZmqSubscriberSocket sub;
  std::string topic = "frame";
  sub.connect(kGatewayLive, std::span{&topic, 1});

  while (true) {
    auto datagram = sub.receive();
    if (!datagram.message.empty()) {
      std::cout << "ASYNC: " << datagram.message << "\n";
    }
  }
}

int main() {
  std::jthread sync(syncCommunication);
  std::jthread async(asyncCommunication);

  return 0;
}

#include "robocin/network/zmq_publisher_socket.h"
#include "robocin/network/zmq_request_reply_socket.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <iostream>
#include <protocols/ui/gateway.grpc.pb.h>
#include <protocols/ui/gateway.pb.h>
#include <protocols/vision/frame.pb.h>
#include <string_view>
#include <thread>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using protocols::ui::GetVisionChunkResponse;
using protocols::ui::ReceiveLiveStreamResponse;
using robocin::ZmqPublisherSocket;
using robocin::ZmqReplySocket;
using robocin::ZmqSubscriberSocket;

using namespace std::chrono_literals;

constexpr auto kSleep = 2s;
constexpr std::string_view kGatewayThirdParties = "ipc:///tmp/gateway-async-backend.ipc";
constexpr std::string_view kVisionRepService = "ipc:///tmp/vision-sync.ipc";
constexpr std::string_view kVisionPubService = "ipc:///tmp/vision-async.ipc";

void messageConsumer() {
  ZmqReplySocket reply;
  reply.bind(kVisionRepService);

  while (true) {
    auto message = reply.receive();

    GetVisionChunkResponse response;
    response.mutable_header()->set_chunk_id(42);

    std::cout << "received\n";
    std::string bytes;
    response.SerializeToString(&bytes);
    reply.send(bytes);
  }
}

void messageProducer() {
  ZmqPublisherSocket pub;
  pub.bind(kVisionPubService);

  while (true) {
    ReceiveLiveStreamResponse response;
    std::string bytes;

    response.SerializeToString(&bytes);
    pub.send("frame", bytes);

    std::this_thread::sleep_for(kSleep);
  }
}

int main() {
  std::jthread consumer(messageConsumer);
  std::jthread producer(messageProducer);
}

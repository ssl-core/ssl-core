#include "referee/messaging/receiver/message_receiver.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/output/log.h>

namespace referee {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqPoller;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::wlog;
using ::robocin::ZmqDatagram;

// NOLINTBEGIN(*naming*, *magic-numbers*)
constexpr auto pRefereePollerTimeoutMs = []() { return /* 240hz ~= */ 4 /*ms*/; };
// NOLINTEND(*naming*, *magic-numbers*)

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> gateway_socket,
                                 std::unique_ptr<IZmqSubscriberSocket> perception_socket,
                                 std::unique_ptr<IZmqPoller> zmq_poller,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    gateway_socket_{std::move(gateway_socket)},
    perception_socket_{std::move(perception_socket)},
    zmq_poller_{std::move(zmq_poller)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {
  ilog("running.");

  std::vector<ZmqDatagram> datagrams{};

  while (datagrams.empty()) {
    zmq_poller_->poll(pRefereePollerTimeoutMs());

    while (true) {
      ZmqDatagram gateway_zmq_datagram = zmq_poller_->receive(*gateway_socket_);
      if (gateway_zmq_datagram.empty()) {
        break;
      }

      datagrams.emplace_back(std::move(gateway_zmq_datagram));
    }

    while (true) {
      ZmqDatagram perception_zmq_datagram = zmq_poller_->receive(*perception_socket_);
      if (perception_zmq_datagram.empty()) {
        break;
      }

      datagrams.emplace_back(std::move(perception_zmq_datagram));
    }

    if (datagrams.empty()) {
      wlog("no datagram received after {} ms.", pRefereePollerTimeoutMs());
    }
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace referee

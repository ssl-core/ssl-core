#include "decision/messaging/receiver/message_receiver.h"

#include "decision/parameters/parameters.h"
#include "payload.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/output/log.h>

namespace decision {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqPoller;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::wlog;
using ::robocin::ZmqDatagram;

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> perception_socket,
                                 std::unique_ptr<IZmqSubscriberSocket> referee_socket,
                                 std::unique_ptr<IZmqPoller> zmq_poller,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    perception_socket_{std::move(perception_socket)},
    referee_socket_{std::move(referee_socket)},
    zmq_poller_{std::move(zmq_poller)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {
  // ilog("running.");

  std::vector<ZmqDatagram> datagrams{};

  while (datagrams.empty()) {
    zmq_poller_->poll(pDecisionPollerTimeoutMs());

    while (true) {
      ZmqDatagram perception_zmq_datagram = zmq_poller_->receive(*perception_socket_);
      if (perception_zmq_datagram.empty()) {
        break;
      }

      datagrams.emplace_back(std::move(perception_zmq_datagram));
    }

    while (true) {
      ZmqDatagram referee_zmq_datagram = zmq_poller_->receive(*referee_socket_);
      if (referee_zmq_datagram.empty()) {
        break;
      }

      datagrams.emplace_back(std::move(referee_zmq_datagram));
    }

    if (datagrams.empty()) {
      // wlog("no datagram received after {} ms.", pDecisionPollerTimeoutMs());
    }
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace decision

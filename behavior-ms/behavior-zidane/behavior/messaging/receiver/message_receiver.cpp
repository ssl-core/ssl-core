#include "behavior/messaging/receiver/message_receiver.h"

#include "behavior/parameters/parameters.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>

namespace behavior {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqPoller;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::wlog;
using ::robocin::ZmqDatagram;

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> perception_socket,
                                 std::unique_ptr<IZmqSubscriberSocket> decision_socket,
                                 std::unique_ptr<IZmqPoller> zmq_poller,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    perception_socket_{std::move(perception_socket)},
    decision_socket_{std::move(decision_socket)},
    zmq_poller_{std::move(zmq_poller)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {
  // ilog("running.");

  std::vector<ZmqDatagram> datagrams{};

  while (datagrams.empty()) {
    zmq_poller_->poll(pBehaviorPollerTimeoutMs());

    // while (true) {
    //   ZmqDatagram perception_zmq_datagram = zmq_poller_->receive(*perception_socket_);
    //   if (perception_zmq_datagram.empty()) {
    //     break;
    //   }

    //   datagrams.emplace_back(std::move(perception_zmq_datagram));
    // }

    while (true) {
      ZmqDatagram decision_zmq_datagram = zmq_poller_->receive(*decision_socket_);
      if (decision_zmq_datagram.empty()) {
        break;
      }

      datagrams.emplace_back(std::move(decision_zmq_datagram));
    }

    if (datagrams.empty()) {
      // wlog("no datagram received after {} ms.", pBehaviorPollerTimeoutMs());
    }
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace behavior

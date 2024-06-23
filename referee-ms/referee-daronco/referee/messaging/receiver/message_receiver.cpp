#include "referee/messaging/receiver/message_receiver.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>

namespace referee {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::ZmqDatagram;

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> perception_socket,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    perception_socket_{std::move(perception_socket)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {
  ilog("running.");

  std::vector<ZmqDatagram> datagrams{};

  // TODO(joseviccruz, aalmds): replace by ZmqPoller.
  while (true) {
    ZmqDatagram datagram = perception_socket_->receive();
    if (datagram.empty()) {
      break;
    }

    datagrams.push_back(datagram);
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace referee

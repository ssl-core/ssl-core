#include "perception/messaging/receiver/message_receiver.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>

namespace perception {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::ZmqDatagram;

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> gateway_socket,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    gateway_socket_{std::move(gateway_socket)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {
  ilog("running.");

  std::vector<ZmqDatagram> datagrams{};

  // TODO(joseviccruz, aalmds): replace by ZmqPoller.
  while (true) {
    ZmqDatagram datagram = gateway_socket_->receive();
    if (datagram.empty()) {
      break;
    }

    datagrams.push_back(datagram);
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace perception

#include "communication/messaging/receiver/message_receiver.h"

#include "communication/parameters/parameters.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/output/log.h>

namespace communication {
namespace {

using ::robocin::ilog;
using ::robocin::IZmqPoller;
using ::robocin::IZmqSubscriberSocket;
using ::robocin::wlog;
using ::robocin::ZmqDatagram;

} // namespace

MessageReceiver::MessageReceiver(std::unique_ptr<IZmqSubscriberSocket> navigation_socket,
                                 std::unique_ptr<IZmqSubscriberSocket> gateway_socket,
                                 std::unique_ptr<IZmqPoller> zmq_poller,
                                 std::unique_ptr<IPayloadMapper> payload_mapper) :
    navigation_socket_{std::move(navigation_socket)},
    gateway_socket_{std::move(gateway_socket)},
    zmq_poller_{std::move(zmq_poller)},
    payload_mapper_{std::move(payload_mapper)} {}

Payload MessageReceiver::receive() {

  std::vector<ZmqDatagram> datagrams{};

  while (datagrams.empty()) {
    zmq_poller_->poll(pCommunicationPollerTimeoutMs());

    while (true) {
      ZmqDatagram navigation_zmq_datagram = zmq_poller_->receive(*navigation_socket_);
      if (navigation_zmq_datagram.empty()) {
        break;
      }
      datagrams.emplace_back(std::move(navigation_zmq_datagram));
    }

    while (true) {
      ZmqDatagram gateway_zmq_datagram = zmq_poller_->receive(*gateway_socket_);
      if (gateway_zmq_datagram.empty()) {
        break;
      }
      // ilog("receiving gateway datagram.");
      datagrams.emplace_back(std::move(gateway_zmq_datagram));
    }

    if (datagrams.empty()) {
      // wlog("no datagram received after {} ms.", pCommunicationPollerTimeoutMs());
    }
  }

  return payload_mapper_->fromZmqDatagrams(datagrams);
}

} // namespace communication

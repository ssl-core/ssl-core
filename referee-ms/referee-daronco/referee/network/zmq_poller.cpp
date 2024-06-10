#include "referee/network/zmq_poller.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <zmq.h>
#include <zmq.hpp>

namespace referee {
namespace {

using ::robocin::ZmqDatagram;
using ::robocin::ZmqSubscriberSocket;

} // namespace

void ZmqPoller::push(ZmqSubscriberSocket& socket) {
  pollitems_.emplace_back(&socket, 0, ZMQ_POLLIN, 0);
}

void ZmqPoller::poll(int64_t timeout_ms) {
  ::zmq::poll(pollitems_, std::chrono::milliseconds{timeout_ms});
}

ZmqDatagram ZmqPoller::receive(ZmqSubscriberSocket& socket) const {
  for (const auto& pollitem : pollitems_) {
    // NOLINTNEXTLINE(*bitwise*)
    if (pollitem.socket == &socket && static_cast<bool>(pollitem.revents & ZMQ_POLLIN)) {
      return socket.receive();
    }
  }
  return ZmqDatagram{};
}

} // namespace referee

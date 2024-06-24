#include "robocin/network/zmq_poller.h"

#if defined(__robocin_lib_zmq) and __robocin_lib_zmq >= 202405L
#if defined(__robocin_lib_cppzmq) and __robocin_lib_cppzmq >= 202405L

#include "robocin/network/zmq_datagram.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <zmq.h>
#include <zmq.hpp>

namespace robocin {

void ZmqPoller::push(IZmqSubscriberSocket& socket) {
  pollitems_.emplace_back(nullptr, socket.fd(), ZMQ_POLLIN, 0);
}

void ZmqPoller::poll(int64_t timeout_ms) {
  ::zmq::poll(pollitems_, std::chrono::milliseconds{timeout_ms});
}

ZmqDatagram ZmqPoller::receive(IZmqSubscriberSocket& socket) const {
  for (const auto& pollitem : pollitems_) {
    // NOLINTNEXTLINE(*bitwise*)
    if (pollitem.fd == socket.fd() && static_cast<bool>(pollitem.revents & ZMQ_POLLIN)) {
      return socket.receive();
    }
  }
  return ZmqDatagram{};
}

} // namespace robocin

#endif
#endif

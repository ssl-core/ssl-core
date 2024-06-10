#ifndef REFEREE_NETWORK_ZMQ_POLLER_H
#define REFEREE_NETWORK_ZMQ_POLLER_H

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_subscriber_socket.h>
#include <vector>
#include <zmq.hpp>

namespace referee {

class ZmqPoller {
 public:
  ZmqPoller() = default;

  void push(::robocin::ZmqSubscriberSocket& socket);
  void poll(int64_t timeout_ms);

  ::robocin::ZmqDatagram receive(::robocin::ZmqSubscriberSocket& socket) const;

 private:
  std::vector<::zmq::pollitem_t> pollitems_;
};

} // namespace referee

#endif // REFEREE_NETWORK_ZMQ_POLLER_H

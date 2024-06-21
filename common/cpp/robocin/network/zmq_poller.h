#ifndef ROBOCIN_NETWORK_ZMQ_POLLER_H
#define ROBOCIN_NETWORK_ZMQ_POLLER_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_zmq) and __robocin_lib_zmq >= 202405L
#if defined(__robocin_lib_cppzmq) and __robocin_lib_cppzmq >= 202405L

#include "robocin/network/zmq_datagram.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <vector>
#include <zmq.hpp>

namespace robocin {

class IZmqPoller { // NOLINT(*member-functions*)
 public:
  IZmqPoller() = default;

  virtual ~IZmqPoller() = default;

  virtual void push(ZmqSubscriberSocket& socket) = 0;
  virtual void poll(int64_t timeout_ms) = 0;

  virtual ZmqDatagram receive(ZmqSubscriberSocket& socket) const = 0;
};

class ZmqPoller : public IZmqPoller {
 public:
  ZmqPoller() = default;

  void push(ZmqSubscriberSocket& socket) override;
  void poll(int64_t timeout_ms) override;

  ZmqDatagram receive(ZmqSubscriberSocket& socket) const override;

 private:
  std::vector<::zmq::pollitem_t> pollitems_;
};

} // namespace robocin

#endif
#endif

#endif // ROBOCIN_NETWORK_ZMQ_POLLER_H

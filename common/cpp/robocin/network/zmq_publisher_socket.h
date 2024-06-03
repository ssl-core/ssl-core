#ifndef ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#define ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_zmq) and __robocin_lib_zmq >= 202405L
#if defined(__robocin_lib_cppzmq) and __robocin_lib_cppzmq >= 202405L

#include "robocin/network/zmq_datagram.h"

#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

#if defined(__robocin_lib_googletest) and __robocin_lib_googletest >= 202405L
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(...)
#endif

namespace robocin {

template <class ZmqSocket, class ZmqContext>
class IZmqPublisherSocket {
 public:
  using datagram_type = ZmqDatagram;

  explicit IZmqPublisherSocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, zmq::socket_type::pub) {}

  void bind(std::string_view address) { socket_.bind(std::string{address}); }

  void send(const datagram_type& datagram) { // NOLINT
    if (zmq::message_t zmq_topic(datagram.topic());
        !socket_.send(zmq_topic, zmq::send_flags::sndmore)) {
      throw std::runtime_error("failed to send topic.");
    }

    if (zmq::message_t zmq_message(datagram.message());
        !socket_.send(zmq_message, zmq::send_flags::dontwait)) {
      throw std::runtime_error("failed to send message.");
    }
  }

  void close() { socket_.close(); }

 private:
  ZmqContext context_;
  ZmqSocket socket_;

  FRIEND_TEST(ZmqPublisherSocketTest, WhenBindIsSucceeded);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenBindThrowsZmqError);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenSendIsSucceeded);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForTopic);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForMessage);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForTopic);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForMessage);
  FRIEND_TEST(ZmqPublisherSocketTest, WhenCloseIsSucceeded);
};

using ZmqPublisherSocket = IZmqPublisherSocket<zmq::socket_t, zmq::context_t>;

} // namespace robocin

#endif
#endif

#endif // ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#ifndef ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#define ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#include <gtest/gtest_prod.h>
#include <optional>
#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

namespace robocin {

template <class ZmqSocket, class ZmqContext>
class IZmqPublisherSocket {
 public:
  explicit IZmqPublisherSocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, ZMQ_PUB) {}

  void bind(std::string_view address) { socket_.bind(std::string{address}); }

  void send(std::string_view topic, std::string_view message) { // NOLINT
    zmq::message_t zmq_topic(topic);
    if (socket_.send(zmq_topic, zmq::send_flags::sndmore) == std::nullopt) {
      throw std::runtime_error("failed to send topic.");
    }

    zmq::message_t zmq_message(message);
    if (socket_.send(zmq_message, zmq::send_flags::dontwait) == std::nullopt) {
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

#endif // ROBOCIN_NETWORK_ZMQ_PUBLISHER_SOCKET_H
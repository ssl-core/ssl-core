#ifndef ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H
#define ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include <gtest/gtest_prod.h>
#include <span>
#include <string>
#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

namespace robocin {

struct ZmqDatagram {
  std::string topic;
  std::string message;

  friend inline bool operator==(const ZmqDatagram& lhs, const ZmqDatagram& rhs) = default;
};

template <class ZmqContext, class ZmqSocket>
class IZmqSubscriberSocket {
 public:
  using receive_type = ZmqDatagram;

  explicit IZmqSubscriberSocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, ZMQ_SUB) {}

  void connect(std::string_view address, std::span<const std::string_view> topics) {
    socket_.connect(std::string{address});
    for (const auto& topic : topics) {
      socket_.set(zmq::sockopt::subscribe, topic);
    }
  }

  receive_type receive() {
    if (zmq::message_t zmq_topic;
        socket_.recv(zmq_topic, zmq::recv_flags::dontwait) != std::nullopt) {
      if (zmq::message_t zmq_result;
          socket_.recv(zmq_result, zmq::recv_flags::dontwait) != std::nullopt) {
        return {.topic = zmq_topic.to_string(), .message = zmq_result.to_string()};
      }
    }

    return {};
  }

  void close() { socket_.close(); }

  [[nodiscard]] int fd() const { return socket_.get(zmq::sockopt::fd); }

 private:
  ZmqContext context_;
  ZmqSocket socket_;

  FRIEND_TEST(ZmqSubscriberSocketTest, WhenConnectIsSucceeded);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenConnectThrowsZmqErrorForAddress);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenConnectThrowsZmqErrorForTopic);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenReceiveIsSucceeded);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenReceiveThrowsZmqErrorForTopic);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenReceiveThrowsZmqErrorForMessage);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenReceiveTopicIsNullopt);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenReceiveMessageIsNullopt);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenFileDescriptionGetterIsSucceeded);
  FRIEND_TEST(ZmqSubscriberSocketTest, WhenCloseIsSucceeded);
};

using ZmqSubscriberSocket = IZmqSubscriberSocket<zmq::context_t, zmq::socket_t>;

} // namespace robocin

#endif // ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

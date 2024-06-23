#ifndef ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H
#define ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_zmq) and __robocin_lib_zmq >= 202405L
#if defined(__robocin_lib_cppzmq) and __robocin_lib_cppzmq >= 202405L

#include "robocin/network/zmq_datagram.h"

#include <string>
#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

#if defined(__robocin_lib_googletest)
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(...)
#endif

namespace robocin {

class IZmqSubscriberSocket {
 public:
  using datagram_type = ZmqDatagram;

  IZmqSubscriberSocket() = default;

  IZmqSubscriberSocket(const IZmqSubscriberSocket&) = delete;
  IZmqSubscriberSocket& operator=(const IZmqSubscriberSocket&) = delete;
  IZmqSubscriberSocket(IZmqSubscriberSocket&&) = default;
  IZmqSubscriberSocket& operator=(IZmqSubscriberSocket&&) = default;

  virtual ~IZmqSubscriberSocket() = default;

  virtual void connect(std::string_view address, std::span<const std::string> topics) = 0;
  virtual void connect(std::string_view address, std::span<const std::string_view> topics) = 0;

  virtual datagram_type receive() = 0;

  virtual void close() = 0;

  [[nodiscard]] virtual int fd() const = 0;
};

template <class ZmqContext, class ZmqSocket>
class AZmqSubscriberSocket : public IZmqSubscriberSocket {
 public:
  explicit AZmqSubscriberSocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, zmq::socket_type::sub) {}

  void connect(std::string_view address, std::span<const std::string> topics) override {
    socket_.connect(std::string{address});
    for (const std::string& topic : topics) {
      socket_.set(zmq::sockopt::subscribe, topic);
    }
  }

  void connect(std::string_view address, std::span<const std::string_view> topics) override {
    socket_.connect(std::string{address});
    for (const std::string_view topic : topics) {
      socket_.set(zmq::sockopt::subscribe, topic);
    }
  }

  datagram_type receive() override {
    if (zmq::message_t zmq_topic; socket_.recv(zmq_topic, zmq::recv_flags::dontwait)) {
      if (zmq::message_t zmq_message; socket_.recv(zmq_message, zmq::recv_flags::dontwait)) {
        return {zmq_topic.to_string(), zmq_message.to_string()};
      }
    }

    return {};
  }

  void close() override { socket_.close(); }

  [[nodiscard]] int fd() const override { return socket_.get(zmq::sockopt::fd); }

 private:
  friend class ZmqPoller;

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

using ZmqSubscriberSocket = AZmqSubscriberSocket<zmq::context_t, zmq::socket_t>;

} // namespace robocin

#endif
#endif

#endif // ROBOCIN_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include "robocin/network/zmq_subscriber_socket.h"

#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <optional>
#include <span>
#include <string_view>
#include <zmq.hpp>

namespace robocin {

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;
using ::testing::Throw;

constexpr int kFileDescriptor = 1;
constexpr std::string_view kAddress = "ipc:///tmp/channel.ipc";
constexpr std::string_view kDefaultTopic = "zmq_topic";
constexpr std::string_view kDefaultMessage = "zmq_message";
constexpr size_t kBytesReceived = 42;

constexpr std::span<const std::string_view> kDefaultTopics{&kDefaultTopic, 1};

class MockZmqContext {
 public:
  explicit MockZmqContext(int /*unused*/) {}
};

class MockZmqSocket {
 public:
  MockZmqSocket(MockZmqContext /*unused*/, zmq::socket_type /*unused*/) {}

  MOCK_METHOD(void, connect, (const std::string&) );
  MOCK_METHOD(void, set, (decltype(zmq::sockopt::subscribe), std::string_view));
  MOCK_METHOD(int, get, (decltype(zmq::sockopt::fd)), (const));
  MOCK_METHOD(zmq::recv_result_t, recv, (zmq::message_t&, zmq::recv_flags));
  MOCK_METHOD(void, close, ());
};

class RecvMockFunctor {
 public:
  explicit RecvMockFunctor(std::string_view message_str) : message_str_(message_str) {}

  zmq::recv_result_t operator()(zmq::message_t& message, zmq::recv_flags /*unused*/) const {
    message = zmq::message_t{message_str_};
    return kBytesReceived;
  }

 private:
  std::string_view message_str_;
};

using MockZmqSubscriberSocket = IZmqSubscriberSocket<MockZmqContext, MockZmqSocket>;

/**
  @brief Matcher used to match the zmq::sockopt parameters which wraps a static constant of
  specified type, similar to std::integral_constant.

  Parameters in zmq that use sockopt are, in reality, specializing templates. So it is not
  necessary to match the value, only the type, and the type provided by the mock is always
  the same as expected by the implementation.
*/
MATCHER(TrueMatcher, "") { return true; }

TEST(ZmqSubscriberSocketTest, WhenConnectIsSucceeded) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, connect(Eq(kAddress)));
  EXPECT_CALL(socket.socket_, set(TrueMatcher(), Eq(kDefaultTopic)));
  socket.connect(kAddress, kDefaultTopics);
}

TEST(ZmqSubscriberSocketTest, WhenConnectThrowsZmqErrorForAddress) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, connect(Eq(kAddress))).WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.connect(kAddress, kDefaultTopics), zmq::error_t);
}

TEST(ZmqSubscriberSocketTest, WhenConnectThrowsZmqErrorForTopic) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, connect(Eq(kAddress)));
  EXPECT_CALL(socket.socket_, set(TrueMatcher(), Eq(kDefaultTopic)))
      .WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.connect(kAddress, kDefaultTopics), zmq::error_t);
}

TEST(ZmqSubscriberSocketTest, WhenReceiveIsSucceeded) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, recv(_, zmq::recv_flags::dontwait))
      .WillOnce(RecvMockFunctor(kDefaultTopic))
      .WillOnce(RecvMockFunctor(kDefaultMessage));

  ZmqDatagram expected{std::string{kDefaultTopic}, std::string{kDefaultMessage}};
  ASSERT_EQ(socket.receive(), expected);
}

TEST(ZmqSubscriberSocketTest, WhenReceiveThrowsZmqErrorForTopic) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, recv(_, zmq::recv_flags::dontwait)).WillOnce(Throw(zmq::error_t{}));
  ASSERT_THROW(socket.receive(), zmq::error_t);
}

TEST(ZmqSubscriberSocketTest, WhenReceiveThrowsZmqErrorForMessage) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, recv(_, zmq::recv_flags::dontwait))
      .WillOnce(RecvMockFunctor(kDefaultTopic))
      .WillOnce(Throw(zmq::error_t{}));
  ASSERT_THROW(socket.receive(), zmq::error_t);
}

TEST(ZmqSubscriberSocketTest, WhenReceiveTopicIsNullopt) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, recv(_, zmq::recv_flags::dontwait)).WillOnce(Return(std::nullopt));
  ASSERT_TRUE(socket.receive().empty());
}

TEST(ZmqSubscriberSocketTest, WhenReceiveMessageIsNullopt) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, recv(_, zmq::recv_flags::dontwait))
      .WillOnce(RecvMockFunctor(kDefaultTopic))
      .WillOnce(Return(std::nullopt));
  ASSERT_TRUE(socket.receive().empty());
}

TEST(ZmqSubscriberSocketTest, WhenFileDescriptionGetterIsSucceeded) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, get(TrueMatcher())).WillOnce(Return(kFileDescriptor));
  ASSERT_EQ(socket.fd(), kFileDescriptor);
}

TEST(ZmqSubscriberSocketTest, WhenCloseIsSucceeded) {
  MockZmqSubscriberSocket socket;

  EXPECT_CALL(socket.socket_, close());
  socket.close();
}

} // namespace robocin

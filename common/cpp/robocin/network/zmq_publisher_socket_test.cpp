#include "robocin/network/zmq_publisher_socket.h"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <zmq.hpp>

namespace robocin {

using ::testing::Eq;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

constexpr size_t kBytesSent = 42;
constexpr std::string_view kDefaultTopic = "zmq_topic";
constexpr std::string_view kDefaultMessage = "zmq_message";

class MockZmqContext {
 public:
  explicit MockZmqContext(int /*unused*/){};
};

class MockZmqSocket {
 public:
  MockZmqSocket(MockZmqContext /*unused*/, int /*unused*/) {}

  MOCK_METHOD(void, bind, (const std::string& addr));
  MOCK_METHOD(zmq::send_result_t, send, (zmq::message_t & msg, zmq::send_flags flags));
  MOCK_METHOD(void, close, ());
};

TEST(ZmqPublisherSocketTest, WhenBindIsSucceeded) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;
  EXPECT_CALL(socket.socket_, bind(Eq("ipc:///tmp/channel.ipc")));
  socket.bind("ipc:///tmp/channel.ipc");
}

TEST(ZmqPublisherSocketTest, WhenBindThrowsZmqError) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;
  EXPECT_CALL(socket.socket_, bind(Eq("ipc:///tmp/channel.ipc"))).WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.bind("ipc:///tmp/channel.ipc"), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendIsSucceeded) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Return(kBytesSent));

  EXPECT_NO_THROW(socket.send(kDefaultTopic, kDefaultMessage));
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForTopic) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.send(kDefaultTopic, kDefaultMessage), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForMessage) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Throw(zmq::error_t{}));

  EXPECT_THROW(socket.send(kDefaultTopic, kDefaultMessage), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForTopic) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(std::nullopt));

  ASSERT_THROW(socket.send(kDefaultTopic, kDefaultMessage), std::runtime_error);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForMessage) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Return(std::nullopt));

  ASSERT_THROW(socket.send(kDefaultTopic, kDefaultMessage), std::runtime_error);
}

TEST(ZmqPublisherSocketTest, WhenCloseIsSucceeded) {
  IZmqPublisherSocket<MockZmqSocket, MockZmqContext> socket;

  EXPECT_CALL(socket.socket_, close());

  socket.close();
}

} // namespace robocin

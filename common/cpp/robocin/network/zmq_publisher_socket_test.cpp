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

constexpr std::string_view kAddress = "ipc:///tmp/channel.ipc";
constexpr std::string_view kDefaultTopic = "zmq_topic";
constexpr std::string_view kDefaultMessage = "zmq_message";
constexpr size_t kBytesSent = 42;

class MockZmqContext {
 public:
  explicit MockZmqContext(int /*unused*/) {};
};

class MockZmqSocket {
 public:
  MockZmqSocket(MockZmqContext /*unused*/, zmq::socket_type /*unused*/) {}

  MOCK_METHOD(void, bind, (const std::string&) );
  MOCK_METHOD(zmq::send_result_t, send, (zmq::message_t&, zmq::send_flags));
  MOCK_METHOD(void, close, ());
};

using MockZmqPublisherSocket = IZmqPublisherSocket<MockZmqSocket, MockZmqContext>;

TEST(ZmqPublisherSocketTest, WhenBindIsSucceeded) {
  MockZmqPublisherSocket socket;

  EXPECT_CALL(socket.socket_, bind(Eq(kAddress)));
  socket.bind(kAddress);
}

TEST(ZmqPublisherSocketTest, WhenBindThrowsZmqError) {
  MockZmqPublisherSocket socket;

  EXPECT_CALL(socket.socket_, bind(Eq(kAddress))).WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.bind(kAddress), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendIsSucceeded) {
  MockZmqPublisherSocket socket;
  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Return(kBytesSent));
  EXPECT_NO_THROW(socket.send({kDefaultTopic, kDefaultMessage}));
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForTopic) {
  MockZmqPublisherSocket socket;
  zmq::message_t zmq_topic(kDefaultTopic);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.send({kDefaultTopic, kDefaultMessage}), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsZmqErrorForMessage) {
  MockZmqPublisherSocket socket;
  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Throw(zmq::error_t{}));
  EXPECT_THROW(socket.send({kDefaultTopic, kDefaultMessage}), zmq::error_t);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForTopic) {
  MockZmqPublisherSocket socket;
  zmq::message_t zmq_topic(kDefaultTopic);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(std::nullopt));
  ASSERT_THROW(socket.send({kDefaultTopic, kDefaultMessage}), std::runtime_error);
}

TEST(ZmqPublisherSocketTest, WhenSendThrowsRuntimeErrorForMessage) {
  MockZmqPublisherSocket socket;
  zmq::message_t zmq_topic(kDefaultTopic);
  zmq::message_t zmq_message(kDefaultMessage);

  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_topic)), Eq(zmq::send_flags::sndmore)))
      .WillOnce(Return(kBytesSent));
  EXPECT_CALL(socket.socket_, send(Eq(std::cref(zmq_message)), Eq(zmq::send_flags::dontwait)))
      .WillOnce(Return(std::nullopt));
  ASSERT_THROW(socket.send({kDefaultTopic, kDefaultMessage}), std::runtime_error);
}

TEST(ZmqPublisherSocketTest, WhenCloseIsSucceeded) {
  MockZmqPublisherSocket socket;

  EXPECT_CALL(socket.socket_, close());
  socket.close();
}

} // namespace robocin

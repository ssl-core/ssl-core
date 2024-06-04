#include "robocin/network/zmq_datagram.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string_view>

namespace robocin {

constexpr std::string_view kTopic = "topic";
constexpr std::string_view kMessage = "message";

TEST(ZmqDatagramTest, WhenZmqDatagramIsNotEmpty) {
  ZmqDatagram datagram(kTopic, kMessage);

  EXPECT_EQ(datagram.topic(), kTopic);
  EXPECT_EQ(datagram.message(), kMessage);
}

TEST(ZmqDatagramTest, WhenEmptyReturnsTrue) {
  ZmqDatagram datagram;

  EXPECT_TRUE(datagram.empty());
}

TEST(ZmqDatagramTest, WhenEmptyReturnsFalse) {
  ZmqDatagram datagram(kTopic, kMessage);

  EXPECT_FALSE(datagram.empty());
}

} // namespace robocin

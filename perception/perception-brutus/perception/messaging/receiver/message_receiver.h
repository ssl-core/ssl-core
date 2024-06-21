#ifndef PERCEPTION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H
#define PERCEPTION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

#include "perception/messaging/receiver/payload.h"
#include "robocin/network/zmq_datagram.h"
#include "robocin/network/zmq_subscriber_socket.h"

#include <string>
#include <vector>

namespace perception {

namespace address {
constexpr std::string_view kRawVisionAddress = "ipc:///tmp/gateway-async-backend.ipc";
constexpr std::array<std::string_view, 1> kVisionTopics = {"vision-third-party"};
} // namespace address

/**
 * @class MessageReceiver
 * @brief Consumes messages from ZeroMQ subscriber sockets.
 *
 * The MessageReceiver class is responsible for subscribing to specified
 * addresses and receiving needed datagrams for Perception execution.
 */
class MessageReceiver {
  using Subscriber = robocin::ZmqSubscriberSocket;
  using Datagram = robocin::ZmqDatagram;

 public:
  explicit MessageReceiver();

  // TODO(#ISSUE_): Implement the real ServiceDiscovery and lookup method.
  struct ServiceDiscovery {
    std::string address;             /**< The address to subscribe to. */
    std::vector<std::string> topics; /**< The topics to subscribe to. */
  };
  [[nodiscard]] static ServiceDiscovery lookup();

  /**
   * @brief Receives available datagrams into a Payload.
   * @return A vector of received datagrams.
   */
  [[nodiscard]] Payload receive();

 private:
  Subscriber vision_sub_; /**< The ZeroMQ subscriber socket for vision topics. */
};

} // namespace perception

#endif // PERCEPTION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

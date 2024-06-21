#ifndef PERCEPTION_NETWORK_MESSAGE_CONSUMER_H
#define PERCEPTION_NETWORK_MESSAGE_CONSUMER_H

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
 * @class MessageConsumer
 * @brief Consumes messages from ZeroMQ subscriber sockets.
 *
 * The MessageConsumer class is responsible for subscribing to specified
 * addresses and receiving needed datagrams for Perception execution.
 */
class MessageConsumer {
  using Subscriber = robocin::ZmqSubscriberSocket;
  using Datagram = robocin::ZmqDatagram;

 public:
  explicit MessageConsumer();

  // TODO(#ISSUE_): Implement the real ServiceDiscovery and lookup method.
  struct ServiceDiscovery {
    std::string address;             /**< The address to subscribe to. */
    std::vector<std::string> topics; /**< The topics to subscribe to. */
  };
  [[nodiscard]] static ServiceDiscovery lookup();

  /**
   * @brief Receives all available datagrams.
   * @return A vector of received datagrams.
   */
  [[nodiscard]] std::vector<Datagram> receiveAll();

 private:
  Subscriber vision_sub_; /**< The ZeroMQ subscriber socket for vision topics. */
};

} // namespace perception

#endif // PERCEPTION_NETWORK_MESSAGE_CONSUMER_H

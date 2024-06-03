#ifndef PERCEPTION_NETWORK_MESSAGE_PRODUCER_H
#define PERCEPTION_NETWORK_MESSAGE_PRODUCER_H

#include "protocols/perception/detection.pb.h"
#include "robocin/network/zmq_datagram.h"
#include "robocin/network/zmq_publisher_socket.h"

#include <string>

namespace perception {

using protocols::perception::Detection;

namespace address {
constexpr std::string_view kDetectionPublisherAddress = "ipc:///tmp/perception-async.ipc";
constexpr std::string_view kDetectionPublisherTopic = "detection";
constexpr std::string_view kDetectionWrapperPublisherTopic = "detection-wrapper";
} // namespace address

/**
 * @class MessageProducer
 * @brief Sends messages to ZeroMQ publisher sockets.
 *
 * The MessageProducer class is responsible for connecting to specified
 * addresses and sending datagrams for subscriber services.
 */
class MessageProducer {
  using Publisher = robocin::ZmqPublisherSocket;
  using Datagram = robocin::ZmqDatagram;

 public:
  explicit MessageProducer();

  // TODO(#ISSUE_N): Implement the real ServiceDiscovery and lookup method.
  struct ServiceDiscovery {
    std::string address; /**< The address to publish to. */
  };
  [[nodiscard]] static ServiceDiscovery lookup();

  /**
   * @brief Sends a `Detection` through the related publishing address and topic.
   * @param detection The detection to be sent.
   */
  void send(const Detection& detection);

  // TODO(#ISSUE_N): Send DetectionWrapper.
  // void send(const DetectionWrapper& wrapper);

 private:
  Publisher detection_pub_; /**< The ZeroMQ publisher socket for detection topics. */
};

} // namespace perception

#endif // PERCEPTION_NETWORK_MESSAGE_PRODUCER_H

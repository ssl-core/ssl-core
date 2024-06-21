#ifndef PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H
#define PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H

#include "robocin/network/zmq_datagram.h"

#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>
#include <variant>
#include <vector>

namespace perception {

// TODO(matheusvtna): Add TelemetryPacket.
// TODO(matheusvtna): Add ParametersPacket.
using RawPacket = protocols::third_party::detection::SSL_WrapperPacket;
using TrackedPacket = protocols::third_party::game_controller::TrackerWrapperPacket;
using PacketVariant = std::variant<RawPacket, TrackedPacket>;

/**
 * @class PayloadFactory
 * @brief Factory class for creating payloads based on the topic and message.
 *
 * The PayloadFactory class provides a static method to create different types
 * of packets based on the topic and message received.
 */
class PayloadPacketFactory {
 public:
  /**
   * @brief Creates an input packet based on the topic and message.
   * @param topic The topic of the datagram.
   * @param message The message content of the datagram.
   * @return A PacketVariant containing the created packet.
   *
   * This method parses the message based on the topic and returns a variant
   * containing the appropriate packet type.
   */
  static PacketVariant createPacket(const std::string_view& topic, const std::string& message);
};

/**
 * @class Payload
 * @brief Manages a collection of input packets.
 *
 * The Payload class is responsible for storing and managing a collection of vision packets,
 * parameters, or any other received message from external components which are created from
 * datagrams.
 */
class Payload {
 public:
  /**
   * @brief Constructs a Payload object from a vector of datagrams.
   * @param datagrams A vector of ZmqDatagram objects to be converted into a Payload with different
   * packets.
   *
   * This constructor initializes the Payload object by processing each datagram
   * and appending the corresponding packet.
   */
  explicit Payload(const std::vector<robocin::ZmqDatagram>& datagrams);

  /**
   * @brief Returns the collection of raw packets.
   * @return A vector of RawPacket objects.
   *
   * This method provides access to the collection of raw packets managed by this class.
   */
  [[nodiscard]] std::vector<RawPacket> rawPackets() const { return raw_packets_; }

  /**
   * @brief Returns the collection of tracked packets.
   * @return A vector of TrackedPacket objects.
   *
   * This method provides access to the collection of tracked packets managed by this class.
   */
  [[nodiscard]] std::vector<TrackedPacket> trackedPackets() const { return tracked_packets_; }

  /**
   * @brief Checks if the payload is empty.
   * @return True if all the payload packets are empty, false otherwise.
   */
  [[nodiscard]] bool isEmpty() const { return raw_packets_.empty() and tracked_packets_.empty(); }

 private:
  std::vector<RawPacket> raw_packets_;         /**< Collection of raw packets. */
  std::vector<TrackedPacket> tracked_packets_; /**< Collection of tracked packets. */

  // TODO(matheusvtna): Add parameters collection.
  // TODO(matheusvtna): Add telemetries collection.

  /**
   * @brief Appends a packet created from the given datagram.
   * @param datagram A ZmqDatagram object to be converted into a packet.
   *
   * This method processes the given datagram and appends the created packet to the appropriate
   * collection.
   */
  void appendPacketFrom(const robocin::ZmqDatagram& datagram);
};

} // namespace perception

#endif // PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H

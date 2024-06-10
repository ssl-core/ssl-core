#ifndef PERCEPTION_CONTROLLER_VISION_PACKETS_H
#define PERCEPTION_CONTROLLER_VISION_PACKETS_H

#include "robocin/network/zmq_datagram.h"

#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>
#include <variant>
#include <vector>

namespace perception {

using RawPacket = protocols::third_party::detection::SSL_WrapperPacket;
using TrackedPacket = protocols::third_party::game_controller::TrackerWrapperPacket;
using PacketVariant = std::variant<RawPacket, TrackedPacket>;

/**
 * @class VisionPacketFactory
 * @brief Factory class for creating vision packets based on the topic and message.
 *
 * The VisionPacketFactory class provides a static method to create different types
 * of vision packets based on the topic and message received.
 */
class VisionPacketFactory {
 public:
  /**
   * @brief Creates a vision packet based on the topic and message.
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
 * @class VisionPackets
 * @brief Manages a collection of vision packets.
 *
 * The VisionPackets class is responsible for storing and managing a collection
 * of vision packets, which are created from datagrams.
 */
class VisionPackets {
 public:
  /**
   * @brief Constructs a VisionPackets object from a vector of datagrams.
   * @param datagrams A vector of ZmqDatagram objects to be converted into vision packets.
   *
   * This constructor initializes the VisionPackets object by processing each datagram
   * and appending the corresponding vision packet.
   */
  explicit VisionPackets(const std::vector<robocin::ZmqDatagram>& datagrams);

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

 private:
  std::vector<RawPacket> raw_packets_;         /**< Collection of raw packets. */
  std::vector<TrackedPacket> tracked_packets_; /**< Collection of tracked packets. */

  /**
   * @brief Appends a vision packet created from the given datagram.
   * @param datagram A ZmqDatagram object to be converted into a vision packet.
   *
   * This method processes the given datagram and appends the created vision packet
   * to the appropriate collection.
   */
  void appendPacketFrom(const robocin::ZmqDatagram& datagram);
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_VISION_PACKETS_H

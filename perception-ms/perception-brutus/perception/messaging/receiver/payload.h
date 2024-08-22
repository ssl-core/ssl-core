#ifndef PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H
#define PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H

#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <span>
#include <vector>

namespace perception {

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
  Payload(
      std::vector<::protocols::third_party::detection::SSL_WrapperPacket> raw_packets,
      std::vector<::protocols::third_party::game_controller::TrackerWrapperPacket> tracked_packets);

  [[nodiscard]] std::span<const ::protocols::third_party::detection::SSL_WrapperPacket>
  getRawPackets() const;

  [[nodiscard]] std::span<const ::protocols::third_party::game_controller::TrackerWrapperPacket>
  getTrackedPackets() const;

  [[nodiscard]] bool empty() const;

 private:
  std::vector<::protocols::third_party::detection::SSL_WrapperPacket> raw_packets_;
  std::vector<::protocols::third_party::game_controller::TrackerWrapperPacket> tracked_packets_;
  // TODO(matheusvtna, joseviccruz): add parameters collection.
  // TODO(matheusvtna, joseviccruz): add telemetries collection.
};

} // namespace perception

#endif // PERCEPTION_MESSAGING_RECEIVER_PAYLOAD_H

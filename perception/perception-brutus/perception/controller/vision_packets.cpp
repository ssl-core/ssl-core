#include "perception/controller/vision_packets.h"

namespace perception {

PacketVariant VisionPacketFactory::createPacket(const std::string_view& topic,
                                                const std::string& message) {
  if (topic == "vision-third-party") {
    RawPacket raw_packet;
    raw_packet.ParseFromString(message);
    return raw_packet;
  }
  // TODO(#ISSUE_N): Create TrackedPacket when the topic is "vision-tracked".

  throw std::invalid_argument("Unexpected topic for ZmqDatagram: " + std::string(topic));
}

VisionPackets::VisionPackets(const std::vector<robocin::ZmqDatagram>& datagrams) {
  for (const auto& datagram : datagrams) {
    appendPacketFrom(datagram);
  }
}

void VisionPackets::appendPacketFrom(const robocin::ZmqDatagram& datagram) {
  std::string_view topic = datagram.topic();
  std::string message = std::string{datagram.message()};

  try {
    PacketVariant packet = VisionPacketFactory::createPacket(topic, message);
    if (std::holds_alternative<RawPacket>(packet)) {
      raw_packets_.push_back(std::get<RawPacket>(packet));
    }
    // TODO(#ISSUE_N): Append TrackedPacket when packet has `TrackedPacket` variant.

  } catch (const std::invalid_argument& e) {
    std::cout << e.what() << std::endl;
  }
}

} // namespace perception

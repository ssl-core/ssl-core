#include "perception/vision_packets/vision_packets.h"

namespace perception {

PacketVariant VisionPacketFactory::createPacket(const std::string_view& topic,
                                                const std::string& message) {
  if (topic == "vision-third-party") {
    RawPacket raw_packet;
    raw_packet.ParseFromString(message);
    return raw_packet;
  }

  if (topic == "tracked-third-party") {
    TrackedPacket tracked_packet;
    tracked_packet.ParseFromString(message);
    return tracked_packet;
  }

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
    if (std::holds_alternative<TrackedPacket>(packet)) {
      tracked_packets_.push_back(std::get<TrackedPacket>(packet));
    }
  } catch (const std::invalid_argument& e) {
    std::cout << e.what() << std::endl;
  }
}

} // namespace perception

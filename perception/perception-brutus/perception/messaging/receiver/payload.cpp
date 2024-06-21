#include "perception/messaging/receiver/payload.h"

namespace perception {

PacketVariant PayloadPacketFactory::createPacket(const std::string_view& topic,
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

Payload::Payload(const std::vector<robocin::ZmqDatagram>& datagrams) {
  for (const auto& datagram : datagrams) {
    appendPacketFrom(datagram);
  }
}

void Payload::appendPacketFrom(const robocin::ZmqDatagram& datagram) {
  std::string_view topic = datagram.topic();
  std::string message = std::string{datagram.message()};

  try {
    PacketVariant packet = PayloadPacketFactory::createPacket(topic, message);
    if (std::holds_alternative<RawPacket>(packet)) {
      raw_packets_.emplace_back(std::get<RawPacket>(packet));
    }
    if (std::holds_alternative<TrackedPacket>(packet)) {
      tracked_packets_.emplace_back(std::get<TrackedPacket>(packet));
    }
  } catch (const std::invalid_argument& e) {
    std::cout << e.what() << std::endl;
  }
}

} // namespace perception

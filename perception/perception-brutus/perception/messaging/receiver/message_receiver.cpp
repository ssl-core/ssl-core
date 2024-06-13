#include "perception/messaging/receiver/message_receiver.h"

namespace perception {

MessageReceiver::MessageReceiver() {
  // TODO(matheusvtna): Dependency injection
  auto [address, topics] = MessageReceiver::lookup();
  vision_sub_.connect(address, topics);
}

Payload MessageReceiver::receive() {
  std::vector<Datagram> datagrams{};
  while (true) {
    auto datagram = vision_sub_.receive();
    if (datagram.empty()) {
      break;
    }
    datagrams.push_back(datagram);
  }
  return Payload(datagrams);
}

MessageReceiver::ServiceDiscovery MessageReceiver::lookup() {
  return {std::string(address::kRawVisionAddress),
          std::vector<std::string>{address::kVisionTopics.begin(), address::kVisionTopics.end()}};
}

} // namespace perception

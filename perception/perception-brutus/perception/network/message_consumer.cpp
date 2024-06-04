#include "perception/network/message_consumer.h"

namespace perception {

MessageConsumer::MessageConsumer() {
  // TODO(#ISSUE_N): Dependency injection
  auto [address, topics] = MessageConsumer::lookup();
  vision_sub_.connect(address, topics);
}

std::vector<MessageConsumer::Datagram> MessageConsumer::receiveAll() {
  std::vector<Datagram> datagrams{};
  while (true) {
    auto datagram = vision_sub_.receive();
    if (datagram.empty()) {
      break;
    }
    datagrams.push_back(datagram);
  }
  return datagrams;
}

MessageConsumer::ServiceDiscovery MessageConsumer::lookup() {
  return {std::string(address::kRawVisionAddress),
          std::vector<std::string>{address::kVisionTopics.begin(), address::kVisionTopics.end()}};
}

} // namespace perception

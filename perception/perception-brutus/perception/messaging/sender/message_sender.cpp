#include "perception/messaging/sender/message_sender.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

MessageSender::MessageSender() {
  // TODO(matheusvtna): Dependency injection
  auto [address] = MessageSender::lookup();
  detection_pub_.bind(address);
}

void MessageSender::send(const Detection& detection) {
  std::string message;
  detection.SerializeToString(&message);
  Datagram datagram{address::kDetectionPublisherTopic, message};
  std::cout << "Sending detection message..." << std::endl;
  detection_pub_.send(datagram);
}

void MessageSender::send(const DetectionWrapper& wrapper) {
  std::string message;
  wrapper.SerializeToString(&message);
  Datagram datagram{address::kDetectionWrapperPublisherTopic, message};
  std::cout << "Sending detection wrapper..." << std::endl;
  detection_pub_.send(datagram);
}

MessageSender::ServiceDiscovery MessageSender::lookup() {
  return {std::string(address::kDetectionPublisherAddress)};
}

} // namespace perception

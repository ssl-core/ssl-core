#include "perception/network/message_producer.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

MessageProducer::MessageProducer() {
  // TODO(#ISSUE_N): Dependency injection
  auto [address] = MessageProducer::lookup();
  detection_pub_.bind(address);
}

void MessageProducer::send(const Detection& detection) {
  std::string message;
  detection.SerializeToString(&message);
  Datagram datagram{address::kDetectionPublisherTopic, message};
  std::cout << "Publishing detection..." << std::endl;
  detection_pub_.send(datagram);
}

// TODO(#ISSUE_N): Create a method to send DetectionWrapper.
// void MessageProducer::send(const DetectionWrapper& wrapper) {
//   std::string message;
//   wrapper.SerializeToString(&message);
//   Datagram datagram{address::kDetectionWrapperPublisherTopic, message};
//   detection_pub_.send(datagram);
// }

MessageProducer::ServiceDiscovery MessageProducer::lookup() {
  return {std::string(address::kDetectionPublisherAddress)};
}

} // namespace perception

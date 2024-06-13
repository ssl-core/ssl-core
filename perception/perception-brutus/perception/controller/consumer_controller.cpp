#include "perception/controller/consumer_controller.h"

namespace perception {

ConsumerController::ConsumerController(Deque& deque) : deque_(deque) {}

void ConsumerController::run() {
  std::cout << "Running consumer controller..." << std::endl;

  while (true) {
    auto payloads = deque_.takeAll();
    if (!payloads.empty()) {
      handleDetectionsFromPayloads(payloads);
    }
  }
}

void ConsumerController::handleDetectionsFromPayloads(const std::vector<Payload>& payloads) {
  std::vector<DetectionPacketVariant> detection_packets{};
  for (const auto& payload : payloads) {
    auto raw_packets = payload.rawPackets();
    auto tracked_packets = payload.trackedPackets();
    detection_packets.insert(detection_packets.end(), raw_packets.begin(), raw_packets.end());
    detection_packets.insert(detection_packets.end(),
                             tracked_packets.begin(),
                             tracked_packets.end());
  }
  DetectionWrapper wrapper = detection_processor_.process({detection_packets});
  message_sender_.send(wrapper.detection());
  message_sender_.send(wrapper);
}

} // namespace perception

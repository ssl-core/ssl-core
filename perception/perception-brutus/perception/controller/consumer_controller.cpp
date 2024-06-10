#include "perception/controller/consumer_controller.h"

namespace perception {

ConsumerController::ConsumerController(Deque& deque) : deque_(deque) {}

void ConsumerController::run() {
  std::cout << "Running consumer controller..." << std::endl;
  while (true) {
    auto packets = deque_.takeAll();
    if (!packets.empty()) {
      DetectionWrapper wrapper = processor_.process(packets);
      message_producer_.send(wrapper.detection());
      message_producer_.send(wrapper);
    }
  }
}

} // namespace perception

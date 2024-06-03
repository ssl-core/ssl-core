#include "perception/controller/consumer_controller.h"

#include <utility>
#include <vector>

namespace perception {

ConsumerController::ConsumerController(std::shared_ptr<Deque> deque) : deque_(std::move(deque)) {
  processor_ = VisionPacketsProcessor();
}

void ConsumerController::run() {
  while (true) {
    auto packets = deque_->takeAll();
    if (!packets.empty()) {
      Detection detection = processor_.process(packets);
      message_producer_.send(detection);
    }
  }
}

} // namespace perception

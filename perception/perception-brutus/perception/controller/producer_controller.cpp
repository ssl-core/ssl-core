#include "perception/controller/producer_controller.h"

#include <utility>

namespace perception {

ProducerController::ProducerController(std::shared_ptr<Deque> deque) : deque_(std::move(deque)) {
  message_consumer_ = MessageConsumer();
}

void ProducerController::run() {
  while (true) {
    auto datagrams = message_consumer_.receiveAll();
    if (!datagrams.empty()) {
      deque_->put(VisionPackets(datagrams));
    }
  }
}

} // namespace perception

#include "perception/controller/producer_controller.h"

namespace perception {

ProducerController::ProducerController(Deque& deque) : deque_(deque) {}

void ProducerController::run() {
  std::cout << "Running producer controller..." << std::endl;
  while (true) {
    auto payload = message_receiver_.receive();
    if (!payload.isEmpty()) {
      deque_.put(payload);
    }
  }
}

} // namespace perception

#include "perception/controller/producer_controller.h"

namespace perception {

ProducerController::ProducerController(Deque& deque) : deque_(deque) {}

void ProducerController::run() {
  std::cout << "Running producer controller..." << std::endl;
  while (true) {
    auto datagrams = message_consumer_.receiveAll();
    if (!datagrams.empty()) {
      std::cout << "Put vision packets into deque." << std::endl;
      deque_.put(VisionPackets(datagrams));
    }
  }
}

} // namespace perception

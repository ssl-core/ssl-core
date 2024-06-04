#include "perception/controller/consumer_controller.h"

namespace perception {

ConsumerController::ConsumerController(Deque& deque) : deque_(deque) {}

void ConsumerController::run() {
  std::cout << "Running consumer controller..." << std::endl;
  while (true) {
    auto packets = deque_.takeAll();
    if (!packets.empty()) {
      std::cout << "Take all vision packets from deque." << std::endl;
      Detection detection = processor_.process(packets);
      std::cout << "Send detection to message producer." << std::endl;
      message_producer_.send(detection);
    }
  }
}

} // namespace perception

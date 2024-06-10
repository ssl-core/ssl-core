#include <memory>
#include <perception/controller/consumer_controller.h>
#include <perception/controller/producer_controller.h>
#include <perception/vision_packets/vision_packets.h>
#include <robocin/concurrency/blocking_deque.h>
#include <thread>

using perception::VisionPackets;
using robocin::BlockingDeque;
using robocin::MutexBlockingDeque;
using VisionPacketsDeque = BlockingDeque<VisionPackets>;
using VisionPacketsMutexDeque = MutexBlockingDeque<VisionPackets>;

using perception::ConsumerController;
using perception::ProducerController;

void producerRun(VisionPacketsDeque& deque) {
  std::unique_ptr<ProducerController> producer_controller
      = std::make_unique<ProducerController>(deque);
  producer_controller->run();
}

void consumerRun(VisionPacketsDeque& deque) {
  std::unique_ptr<ConsumerController> consumer_controller
      = std::make_unique<ConsumerController>(deque);
  consumer_controller->run();
}

int main() {
  std::cout << "Perception is runnning!" << std::endl;
  auto deque = VisionPacketsMutexDeque();

  std::jthread producer_thread(producerRun, std::ref(deque));
  std::jthread consumer_thread(consumerRun, std::ref(deque));

  return 0;
}

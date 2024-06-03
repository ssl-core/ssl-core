#ifndef PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H
#define PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H

#include "perception/controller/icontroller.h"
#include "perception/controller/vision_packets.h"
#include "perception/network/message_consumer.h"
#include "robocin/concurrency/blocking_deque.h"

#include <memory>

namespace perception {

/**
 * @class ProducerController
 * @brief Manages the production of vision packets and adds them to a blocking deque.
 *
 * The ProducerController class is responsible for consuming messages, processing
 * them into vision packets, and adding these packets to a blocking deque.
 */
class ProducerController : IController {
  using Deque = robocin::BlockingDeque<VisionPackets>;

 public:
  /**
   * @brief Constructs a ProducerController object.
   * @param deque Pointer to a blocking deque where vision packets will be added.
   */
  explicit ProducerController(std::shared_ptr<Deque> deque);

  /**
   * @brief Starts the controller's run loop to consume and process messages.
   *
   * This method overrides the run method of the IController interface. It continuously
   * consumes messages, processes them into vision packets, and adds these packets to the deque.
   */
  void run() override;

 private:
  std::shared_ptr<Deque>
      deque_; /**< Pointer to the blocking deque where vision packets will be added. */
  MessageConsumer message_consumer_; /**< Consumer for handling incoming messages. */
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H

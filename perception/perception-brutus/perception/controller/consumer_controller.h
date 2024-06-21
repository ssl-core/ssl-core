#ifndef PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H
#define PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "perception/controller/icontroller.h"
#include "perception/controller/vision_packets_processor.h"
#include "perception/network/message_producer.h"
#include "robocin/concurrency/blocking_deque.h"

namespace perception {

/**
 * @class ConsumerController
 * @brief Manages the consumption of vision packets from a deque and processes them for publication.
 *
 * The ConsumerController class is responsible for fetching vision packets from a blocking deque,
 * processing these packets, and publishing the processed results using a ZeroMQ publisher socket.
 */
class ConsumerController : IController {
  using Datagram = robocin::ZmqDatagram;
  using Deque = robocin::BlockingDeque<VisionPackets>;

 public:
  /**
   * @brief Constructs a ConsumerController object.
   * @param deque Reference to a blocking deque from which vision packets will be consumed.
   */
  explicit ConsumerController(Deque& deque);

  /**
   * @brief Starts the controller's run loop to consume and process vision packets.
   *
   * This method overrides the run method of the IController interface. It continuously
   * fetches vision packets from the deque, processes them using VisionPacketsProcessor,
   * and publishes the processed packets.
   */
  void run() override;

 private:
  Deque& deque_; /**< Reference to the blocking deque containing vision packets to be consumed. */
  VisionPacketsProcessor processor_; /**< Processor for handling vision packets. */
  MessageProducer message_producer_; /**< Producer for handling outgoing messages. */
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

#ifndef PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H
#define PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "perception/controller/icontroller.h"
#include "perception/messaging/receiver/payload.h"
#include "perception/messaging/sender/message_sender.h"
#include "perception/processing/detection_processor.h"
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
  using Deque = robocin::BlockingDeque<Payload>;

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
   * fetches vision packets from the deque, processes them using PayloadProcessor,
   * and publishes the processed packets.
   */
  void run() override;

 private:
  Deque& deque_; /**< Reference to the blocking deque containing vision packets to be consumed. */
  DetectionProcessor detection_processor_; /**< Processor for handling detection packets. */
  MessageSender message_sender_;           /**< Producer for handling outgoing messages. */

  /**
   * @brief Handles detections from payloads by calling the detection processor and publish the
   * results through the associated MessageSender.
   * @param payloads A vector of Payload objects containing detection data.
   */
  void handleDetectionsFromPayloads(const std::vector<Payload>& payloads);
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

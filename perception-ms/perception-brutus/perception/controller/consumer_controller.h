#ifndef PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H
#define PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "perception/controller/icontroller.h"
#include "perception/messaging/receiver/payload.h"
#include "perception/messaging/sender/message_sender.h"
#include "perception/processing/detection_processor.h"

#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace perception {

/**
 * @class ConsumerController
 * @brief Manages the consumption of vision packets from a deque and processes them for publication.
 *
 * The ConsumerController class is responsible for fetching vision packets from a blocking deque,
 * processing these packets, and publishing the processed results using a ZeroMQ publisher socket.
 */
class ConsumerController : public IController {
 public:
  ConsumerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                     std::unique_ptr<IDetectionProcessor> detection_processor,
                     std::unique_ptr<IMessageSender> message_sender);

  /**
   * @brief Starts the controller's run loop to consume and process vision packets.
   *
   * This method overrides the run method of the IController interface. It continuously
   * fetches vision packets from the deque, processes them using PayloadProcessor,
   * and publishes the processed packets.
   */
  void run() override;

 private:
  void exec(std::span<const Payload> payloads);

  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<IDetectionProcessor> detection_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_CONSUMER_CONTROLLER_H

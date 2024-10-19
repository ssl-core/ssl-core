#ifndef BEHAVIOR_CONTROLLER_CONSUMER_CONTROLLER_H
#define BEHAVIOR_CONTROLLER_CONSUMER_CONTROLLER_H

#include "behavior/controller/icontroller.h"
#include "behavior/messaging/receiver/payload.h"
#include "behavior/messaging/sender/message_sender.h"
#include "behavior/processing/behavior_processor.h"

#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/parameters/parameters.h>
namespace behavior {

// TODO: adjust this documentation to behavior
/**
 * @class ConsumerController
 * @brief Manages the consumption of vision packets from a deque and processes them for publication.
 *
 * The ConsumerController class is responsible for fetching vision packets from a blocking deque,
 * processing these packets, and publishing the processed results using a ZeroMQ publisher socket.
 */
class ConsumerController : public IController {
 public:
  ConsumerController(
      ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
      std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine,
      std::unique_ptr<IBehaviorProcessor> behavior_processor,
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
  std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine_;
  std::unique_ptr<IBehaviorProcessor> behavior_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace behavior

#endif // BEHAVIOR_CONTROLLER_CONSUMER_CONTROLLER_H

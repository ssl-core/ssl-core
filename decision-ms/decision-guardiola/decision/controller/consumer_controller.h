#ifndef DECISION_CONTROLLER_CONSUMER_CONTROLLER_H
#define DECISION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "decision/controller/icontroller.h"
#include "decision/messaging/receiver/payload.h"
#include "decision/messaging/sender/message_sender.h"
#include "decision/processing/decision_processor.h"

#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/parameters/parameters.h>
namespace decision {

// TODO: adjust this documentation to decision
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
      std::unique_ptr<IDecisionProcessor> decision_processor,
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
  std::unique_ptr<IDecisionProcessor> decision_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace decision

#endif // DECISION_CONTROLLER_CONSUMER_CONTROLLER_H

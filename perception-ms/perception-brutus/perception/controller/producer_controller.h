#ifndef PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H
#define PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H

#include "perception/controller/icontroller.h"
#include "perception/messaging/receiver/message_receiver.h"

#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace perception {

class ProducerController : public IController {
 public:
  explicit ProducerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                              std::unique_ptr<IMessageReceiver> message_receiver);

  /**
   * @brief Starts the controller's run loop to consume and process messages.
   *
   * This method overrides the run method of the IController interface. It continuously
   * consumes messages, processes them into vision packets, and adds these packets to the deque.
   */
  void run() override;

 private:
  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<IMessageReceiver> message_receiver_;
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_PRODUCER_CONTROLLER_H

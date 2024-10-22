#ifndef NAVIGATION_CONTROLLER_PRODUCER_CONTROLLER_H
#define NAVIGATION_CONTROLLER_PRODUCER_CONTROLLER_H

#include "navigation/controller/icontroller.h"
#include "navigation/messaging/receiver/message_receiver.h"

#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace navigation {

class ProducerController : public IController {
 public:
  explicit ProducerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                              std::unique_ptr<IMessageReceiver> message_receiver);

  void run() override;

 private:
  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<IMessageReceiver> message_receiver_;
};

} // namespace navigation

#endif // NAVIGATION_CONTROLLER_PRODUCER_CONTROLLER_H


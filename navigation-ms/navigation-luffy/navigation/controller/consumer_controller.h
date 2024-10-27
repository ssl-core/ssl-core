#ifndef NAVIGATION_CONTROLLER_CONSUMER_CONTROLLER_H
#define NAVIGATION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "navigation/controller/icontroller.h"
#include "navigation/messaging/receiver/payload.h"
#include "navigation/messaging/sender/message_sender.h"
#include "navigation/processing/navigation_processor.h"

#include <memory>
#include <protocols/navigation/navigation.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace navigation {

class ConsumerController : public IController {
 public:
  ConsumerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                     std::unique_ptr<INavigationProcessor> navigation_processor,
                     std::unique_ptr<IMessageSender> message_sender);

  void run() override;

 private:
  void exec(std::span<const Payload> payloads);

  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<INavigationProcessor> navigation_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace navigation

#endif // NAVIGATION_CONTROLLER_CONSUMER_CONTROLLER_H

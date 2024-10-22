#ifndef COMMUNICATION_CONTROLLER_CONSUMER_CONTROLLER_H
#define COMMUNICATION_CONTROLLER_CONSUMER_CONTROLLER_H

#include "communication/controller/icontroller.h"
#include "communication/messaging/receiver/payload.h"
#include "communication/messaging/sender/message_sender.h"
#include "communication/processing/communication_processor.h"

#include <memory>
#include <protocols/communication/robot_info.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace communication {

class ConsumerController : public IController {
 public:
  ConsumerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                     std::unique_ptr<ICommunicationProcessor> communication_processor,
                     std::unique_ptr<IMessageSender> message_sender);

  void run() override;

 private:
  void exec(std::span<const Payload> payloads);

  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<ICommunicationProcessor> communication_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace communication

#endif // COMMUNICATION_CONTROLLER_CONSUMER_CONTROLLER_H

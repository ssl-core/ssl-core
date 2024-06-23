#ifndef REFEREE_CONTROLLER_CONSUMER_CONTROLLER_H
#define REFEREE_CONTROLLER_CONSUMER_CONTROLLER_H

#include "referee/controller/icontroller.h"
#include "referee/messaging/receiver/payload.h"
#include "referee/messaging/sender/message_sender.h"
#include "referee/processing/referee_processor.h"

#include <protocols/referee/game_status.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>

namespace referee {

class ConsumerController : public IController {
 public:
  ConsumerController(::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
                     std::unique_ptr<IRefereeProcessor> referee_processor,
                     std::unique_ptr<IMessageSender> message_sender);

  void run() override;

 private:
  void exec(std::span<const Payload> payloads);

  ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages_;
  std::unique_ptr<IRefereeProcessor> referee_processor_;
  std::unique_ptr<IMessageSender> message_sender_;
};

} // namespace referee

#endif // REFEREE_CONTROLLER_CONSUMER_CONTROLLER_H

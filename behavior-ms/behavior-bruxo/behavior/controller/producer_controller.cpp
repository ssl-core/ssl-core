#include "behavior/controller/producer_controller.h"

#include <robocin/output/log.h>

namespace behavior {
namespace {

using ::robocin::IConcurrentQueue;
using ::robocin::ilog;

} // namespace

ProducerController::ProducerController(
    ::robocin::object_ptr<::robocin::IConcurrentQueue<Payload>> messages,
    std::unique_ptr<IMessageReceiver> message_receiver) :
    messages_{messages},
    message_receiver_{std::move(message_receiver)} {}

void ProducerController::run() {
  // ilog("running.");

  while (true) {
    if (Payload payload = message_receiver_->receive(); !payload.empty()) {
      // ilog("Received something");
      messages_->push(std::move(payload));
    }
  }
}

} // namespace behavior

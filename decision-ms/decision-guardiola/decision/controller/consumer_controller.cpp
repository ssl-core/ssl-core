#include "decision/controller/consumer_controller.h"

#include "decision/messaging/receiver/payload.h"

#include <optional>
#include <protocols/decision/decision.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace decision {

namespace {

namespace parameters = ::robocin::parameters;

using ::robocin::IConcurrentQueue;
using ::robocin::ilog;
using ::robocin::object_ptr;

namespace rc {

using ::protocols::decision::Decision;

} // namespace rc

} // namespace

ConsumerController::ConsumerController(
    object_ptr<IConcurrentQueue<Payload>> messages,
    std::unique_ptr<parameters::IHandlerEngine> parameters_handler_engine,
    std::unique_ptr<IDecisionProcessor> decision_processor,
    std::unique_ptr<IMessageSender> message_sender) :
    messages_{messages},
    parameters_handler_engine_{std::move(parameters_handler_engine)},
    decision_processor_{std::move(decision_processor)},
    message_sender_{std::move(message_sender)} {}

void ConsumerController::run() {
  // ilog("running.");

  while (true) {
    std::vector<Payload> payloads = messages_->dequeue_all();
    exec(payloads);
  }
}

void ConsumerController::exec(std::span<const Payload> payloads) {
  // ilog("payloads {} empty.", payloads.empty() ? "is" : "isn't");

  if (payloads.empty()) {
    return;
  }

  // TODO(joseviccruz): update parameters here from Payload.
  // parameters_handler_engine_->update(parameters_values);

  std::optional<rc::Decision> decision = decision_processor_->process(payloads);

  if (decision != std::nullopt) {
    message_sender_->send(*decision);
  }
}

} // namespace decision

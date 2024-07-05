#include "referee/controller/consumer_controller.h"

#include "referee/messaging/receiver/payload.h"

#include <protocols/referee/game_status.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace referee {

namespace {

namespace parameters = ::robocin::parameters;
namespace detection_util = ::robocin::detection_util;

using ::robocin::IConcurrentQueue;
using ::robocin::ilog;
using ::robocin::object_ptr;

namespace rc {

using ::protocols::referee::GameStatus;

} // namespace rc

} // namespace

ConsumerController::ConsumerController(
    object_ptr<IConcurrentQueue<Payload>> messages,
    std::unique_ptr<parameters::IHandlerEngine> parameters_handler_engine,
    std::unique_ptr<detection_util::IClockEngine> clock_engine,
    std::unique_ptr<IRefereeProcessor> referee_processor,
    std::unique_ptr<IMessageSender> message_sender) :
    messages_{messages},
    parameters_handler_engine_{std::move(parameters_handler_engine)},
    clock_engine_{std::move(clock_engine)},
    referee_processor_{std::move(referee_processor)},
    message_sender_{std::move(message_sender)} {}

void ConsumerController::run() {
  ilog("running.");

  while (true) {
    std::vector<Payload> payloads = messages_->dequeue_all();

    // TODO(joseviccruz): update parameters here from Payload.
    // parameters_handler_engine_->update(parameters_values);

    // TODO(joseviccruz): update detection clock here from Payload.
    // clock_engine_->update(framerate, timestamp);

    exec(payloads);
  }
}

void ConsumerController::exec(std::span<const Payload> payloads) {
  ilog("payloads {} empty.", payloads.empty() ? "is" : "isn't");

  if (payloads.empty()) {
    return;
  }

  if (std::optional<rc::GameStatus> game_status = referee_processor_->process(payloads);
      game_status != std::nullopt) {
    message_sender_->send(*game_status);
  }
}

} // namespace referee

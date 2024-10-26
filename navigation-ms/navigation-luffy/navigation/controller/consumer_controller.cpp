#include "navigation/controller/consumer_controller.h"

#include "navigation/messaging/receiver/payload.h"

#include <protocols/navigation/navigation.pb.h>
#include <robocin/concurrency/concurrent_queue.h>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace navigation {

namespace {

namespace parameters = ::robocin::parameters;

using ::robocin::IConcurrentQueue;
using ::robocin::ilog;
using ::robocin::object_ptr;

namespace rc {

using ::protocols::navigation::Navigation;

} // namespace rc

} // namespace

ConsumerController::ConsumerController(object_ptr<IConcurrentQueue<Payload>> messages,
                                       std::unique_ptr<INavigationProcessor> navigation_processor,
                                       std::unique_ptr<IMessageSender> message_sender) :
    messages_{messages},
    navigation_processor_{std::move(navigation_processor)},
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

  if (std::optional<rc::Navigation> navigation_msg = navigation_processor_->process(payloads);
      navigation_msg != std::nullopt) {
    message_sender_->send(*navigation_msg);
  }
}

} // namespace navigation

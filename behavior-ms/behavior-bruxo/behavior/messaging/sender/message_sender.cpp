
#include "behavior/messaging/sender/message_sender.h"

#include <protocols/behavior/behavior_unification.pb.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace behavior {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::ilog;

namespace rc {

using ::protocols::behavior::unification::Behavior;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> behavior_socket) :
    behavior_socket_{std::move(behavior_socket)} {}

void MessageSender::send(const rc::Behavior& behavior) {
  // ilog("sending... {}", behavior.DebugString());

  behavior_socket_->send({
      service_discovery::kBehaviorUnificationTopic,
      behavior.SerializeAsString(),
  });
}

} // namespace behavior

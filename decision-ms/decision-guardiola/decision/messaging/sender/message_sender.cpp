
#include "decision/messaging/sender/message_sender.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace decision {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::ilog;

namespace rc {

using ::protocols::decision::Decision;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> decision_socket) :
    decision_socket_{std::move(decision_socket)} {}

void MessageSender::send(const rc::Decision& decision) {
  // ilog("sending... {}", decision.DebugString());

  decision_socket_->send({
      service_discovery::kDecisionTopic,
      decision.SerializeAsString(),
  });
}

} // namespace decision

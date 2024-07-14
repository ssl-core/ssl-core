#include "referee/messaging/sender/message_sender.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace referee {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::ilog;

namespace rc {

using ::protocols::referee::GameStatus;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket) :
    detection_socket_{std::move(detection_socket)} {}

void MessageSender::send(const rc::GameStatus& game_status) {
  // ilog("sending... {}", game_status.DebugString());

  detection_socket_->send({
      service_discovery::kRefereeGameStatusTopic,
      game_status.SerializeAsString(),
  });
}

} // namespace referee

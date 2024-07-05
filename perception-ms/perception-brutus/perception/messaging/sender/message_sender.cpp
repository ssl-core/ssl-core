#include "perception/messaging/sender/message_sender.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace perception {

namespace {

namespace service_discovery = ::robocin::service_discovery;

using ::robocin::ilog;

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::perception::DetectionWrapper;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket) :
    detection_socket_{std::move(detection_socket)} {}

void MessageSender::send(const rc::Detection& detection) {
  ilog("sending... {}", detection.DebugString());

  detection_socket_->send({
      service_discovery::kPerceptionDetectionTopic,
      detection.SerializeAsString(),
  });
}

void MessageSender::send(const rc::DetectionWrapper& detection_wrapper) {
  ilog("sending... {}", detection_wrapper.DebugString());

  detection_socket_->send({
      service_discovery::kPerceptionDetectionWrapperTopic,
      detection_wrapper.SerializeAsString(),
  });
}

} // namespace perception

#include "perception/messaging/sender/message_sender.h"

#include "perception/messaging/internal/messaging_internal.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>

namespace perception {

namespace {

using ::robocin::ilog;

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::perception::DetectionWrapper;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket) :
    detection_socket_{std::move(detection_socket)} {}

void MessageSender::send(const rc::Detection& detection) {
  ilog("sending...");

  detection_socket_->send({
      messaging_internal::kDetectionTopic,
      detection.SerializeAsString(),
  });
}

void MessageSender::send(const rc::DetectionWrapper& detection_wrapper) {
  ilog("sending...");

  detection_socket_->send({
      messaging_internal::kDetectionWrapperTopic,
      detection_wrapper.SerializeAsString(),
  });
}

} // namespace perception

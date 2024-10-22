#include "communication/messaging/sender/message_sender.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace communication {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::ilog;

namespace rc {

using ::protocols::communication::RobotInfo;

} // namespace rc

} // namespace

MessageSender::MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> communication_socket,
                             std::unique_ptr<::robocin::IUdpMulticastSocketSender> robot_socket) :
    communication_socket_{std::move(communication_socket)},
    robot_socket_{std::move(robot_socket)} {}

void MessageSender::send(const rc::RobotInfo& robot_command) {
  // ilog("sending... {}", robot_command.DebugString());

  communication_socket_->send({
      service_discovery::kCommunicationCommandTopic,
      robot_command.SerializeAsString(),
  });
  robot_socket_->send(robot_command.SerializeAsString());

  // ilog("Message sent");
}

} // namespace communication

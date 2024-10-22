#include "communication/processing/robot_command/robot_command_mapper.h"

#include <google/protobuf/util/time_util.h>
#include <protocols/navigation/navigation.pb.h>
#include <protocols/communication/robot_info.pb.h> 
#include <protocols/common/robot_id.pb.h>
#include <protocols/common/message_type.pb.h>
#include <protocols/common/robot_velocity.pb.h>
#include <protocols/common/robot_kick.pb.h>
#include <protocols/common/robot_dribbler.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>
#include <utility>

namespace communication {
namespace {

using ::google::protobuf::util::TimeUtil;
using ::robocin::object_ptr;

namespace rc {
    
using ::protocols::navigation::Navigation;
using ::protocols::communication::RobotInfo;
using ::protocols::communication::Command;
using ::protocols::communication::Flags;
using ::protocols::common::RobotId;
using ::protocols::common::MessageType;
using ::protocols::common::RobotVelocity;
using ::protocols::common::RobotKick;
using ::protocols::common::RobotDribbler;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

} // namespace

RobotCommandMapper::RobotCommandMapper(){}

rc::RobotInfo RobotCommandMapper::fromNavigationAndReferee(const tp::Referee& referee, const rc::Navigation& navigation) {
    rc::RobotInfo result;
    rc::Command* command = result.mutable_command();
    rc::RobotId* robot_id = command->mutable_robot_id();
    rc::RobotVelocity robot_velocity;
    rc::RobotKick::KickCommand kick_command;
    rc::RobotDribbler::DribblerCommand dribbler_command;
    rc::Flags robot_flag;

    command->set_msg_type(rc::MessageType::SSL_SPEED);
    
    robot_id->CopyFrom(navigation.output(navigation.output_size() - 1).robot_id());

    // *command->mutable_ref_command()->CopyFrom(referee.ref_command());

    robot_velocity.mutable_velocity()->set_x(navigation.output(navigation.output_size() - 1).forward_velocity());
    robot_velocity.mutable_velocity()->set_y(navigation.output(navigation.output_size() - 1).left_velocity());
    robot_velocity.set_angular_velocity(navigation.output(navigation.output_size() - 1).angular_velocity());
    *command->mutable_robot_velocity() = robot_velocity;

    kick_command.set_kick_strength(5.0);
    kick_command.set_is_front(false);
    kick_command.set_is_chip(false);
    kick_command.set_charge_capacitor(false);
    kick_command.set_is_bypass_ir(false);
    *command->mutable_kick_command() = kick_command;

    dribbler_command.set_dribbler_speed(0);
    dribbler_command.set_is_active(false);
    *command->mutable_dribbler_command() = dribbler_command;

    robot_flag.set_robot_locked(false);
    robot_flag.set_critical_move(false);
    robot_flag.set_global_speed(false);
    *command->mutable_robot_flags() = robot_flag;

    return result;
}

} // namespace communication

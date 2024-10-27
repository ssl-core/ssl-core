#ifndef COMMUNICATION_PROCESSING_ROBOT_COMMAND_ROBOT_COMMAND_MAPPER_H
#define COMMUNICATION_PROCESSING_ROBOT_COMMAND_ROBOT_COMMAND_MAPPER_H

#include <protocols/communication/robot_info.pb.h>
#include <protocols/navigation/navigation.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <robocin/memory/object_ptr.h>

namespace communication {

class IRobotCommandMapper {
 public:
  IRobotCommandMapper() = default;

  IRobotCommandMapper(const IRobotCommandMapper&) = delete;
  IRobotCommandMapper& operator=(const IRobotCommandMapper&) = delete;
  IRobotCommandMapper(IRobotCommandMapper&&) = default;
  IRobotCommandMapper& operator=(IRobotCommandMapper&&) = default;

  virtual ~IRobotCommandMapper() = default;

  virtual ::protocols::communication::RobotInfo
  fromNavigationAndReferee(const ::protocols::third_party::game_controller::Referee& referee,
                           const ::protocols::navigation::Navigation& navigation)
      = 0;
};

class RobotCommandMapper : public IRobotCommandMapper {
 public:
  RobotCommandMapper();

  ::protocols::communication::RobotInfo
  fromNavigationAndReferee(const ::protocols::third_party::game_controller::Referee& referee,
                           const ::protocols::navigation::Navigation& navigation) override;
};

} // namespace communication

#endif // COMMUNICATION_PROCESSING_ROBOT_COMMAND_ROBOT_COMMAND_MAPPER_H

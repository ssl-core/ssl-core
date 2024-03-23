module;

#include "protocols/common/robot_id.pb.h"
#include "protocols/vision/frame.pb.h"

export module decision.mappers:robot_pb_to_robot_mapper;

import decision.models;
import decision.geometry;

export namespace decision {

class RobotPbToRobotMapper {
  static Robot toRobot(const ::protocols::vision::Robot& robot_pb) {
    return Robot::Builder{}
        .setId(robot_pb.robot_id().number())
        .setColor(toColor(robot_pb.robot_id().color()))
        .setPosition(Point2Df{robot_pb.position()})
        .setAngle(robot_pb.angle())
        .build();
  }

 private:
  static Robot::Color toColor(::protocols::common::RobotId::Color color) {
    switch (color) {
      case ::protocols::common::RobotId::COLOR_BLUE: return Robot::Color::Blue;
      case ::protocols::common::RobotId::COLOR_YELLOW: return Robot::Color::Yellow;
      default: return Robot::Color::Unspecified;
    }
    std::unreachable();
  }
};

} // namespace decision

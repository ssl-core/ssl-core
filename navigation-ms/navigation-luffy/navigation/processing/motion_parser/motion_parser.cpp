#include "navigation/processing/motion_parser/motion_parser.h"

#include <robocin/geometry/point2d.h>
#include <robocin/utility/angular.h>

namespace navigation {

MotionParser::MotionParser() {}

RobotMove MotionParser::fromGoToPoint(const ::protocols::behavior::GoToPoint& go_to_point,
                                      const ::protocols::perception::Robot& robot) {

  robocin::Point2D robot_position = robocin::Point2D(robot.position().x(), robot.position().y());
  robocin::Point2D target_position
      = robocin::Point2D(go_to_point.target().x(), go_to_point.target().y());

  double distanceToGoal = robot_position.distanceTo(target_position);
  double max_velocity = 2.2;

  auto smallestAngleDiff = [](double lhs, double rhs) {
    double angle = fmod(rhs - lhs, 2 * PI);
    if (angle >= PI) {
      return angle - 2 * PI;
    } else if (angle < -PI) {
      return angle + 2 * PI;
    }
    return angle;
  };

  auto fromPolar = [](auto length, auto angle) {
    return std::make_pair(std::cos(angle) * length, std::sin(angle) * length);
  };

  if (distanceToGoal > 50) {
    auto theta = (target_position - robot_position).angle();

    auto dTheta = smallestAngleDiff(robot.angle(), go_to_point.target_angle());
    auto v_prop = std::abs(smallestAngleDiff(PI - 0.1, dTheta)) * max_velocity / (PI - 0.1);

    return RobotMove(fromPolar(v_prop, theta), 5 * dTheta);

  } else {
    // anglePID()
    auto dTheta = smallestAngleDiff(robot.angle(), go_to_point.target_angle());

    return RobotMove(5 * dTheta);
  }
}

RobotMove
MotionParser::fromRotateInPoint(const ::protocols::behavior::RotateInPoint& rotate_in_point,
                                const ::protocols::perception::Robot& robot) {

  // PROCESSAMENTO DO ROTATEINPOINT
  return RobotMove{};
}

RobotMove MotionParser::fromRotateOnSelf(const ::protocols::behavior::RotateOnSelf& rotate_on_self,
                                         const ::protocols::perception::Robot& robot) {

  auto d_theta = robocin::smallestAngleDiff<double>(robot.angle(), rotate_on_self.target_angle());
  robocin::Point2Dd velocity
      = robocin::Point2Dd{rotate_on_self.velocity().x(), rotate_on_self.velocity().y()};

  return RobotMove{velocity, rotate_on_self.kp() * d_theta};
}

} // namespace navigation

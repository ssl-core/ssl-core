#include "perception/raw_detection/filter/robot_filter.h"

#include "perception/raw_detection/entities/raw_robot.h"

namespace perception {

RobotFilter::RobotFilter(const RawRobot& robot) : last_robot_(robot) { update(robot); };

void RobotFilter::update(const RawRobot& robot) {
  // TODO(#ISSUE_N): Create the RobotFilter update method.
  last_robot_ = robot;
}

RawRobot RobotFilter::getRobot() const { return last_robot_; }

} // namespace perception

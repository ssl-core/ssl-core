#include "perception/filter/robot_filter.h"

#include "perception/entities/raw_robot.h"

namespace perception {

void RobotFilter::update(const RawRobot& robot) {
  // TODO(#ISSUE_N): Create the RobotFilter update method.
  last_robot_ = robot;
}

RawRobot RobotFilter::getRobot() const { return last_robot_; }

} // namespace perception

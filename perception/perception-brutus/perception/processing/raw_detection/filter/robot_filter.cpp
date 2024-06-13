#include "perception/processing/raw_detection/filter/robot_filter.h"

namespace perception {

RobotFilter::RobotFilter(const RawRobot& robot) : last_robot_(robot) { update(robot); };

void RobotFilter::update(const RawRobot& robot) {
  // TODO(matheusvtna): Create the RobotFilter update method.
  last_robot_ = robot;
}

RawRobot RobotFilter::getRobot() const { return last_robot_; }

} // namespace perception

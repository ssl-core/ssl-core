#include "perception/processing/raw_detection/filters/robot_filter.h"

namespace perception {

void RobotFilter::update(const RawRobot& robot) {
  // TODO(matheusvtna): Create the RobotFilter update method.
  last_robot_ = robot;
}

std::optional<RawRobot> RobotFilter::getRobot() const { return last_robot_; }

} // namespace perception

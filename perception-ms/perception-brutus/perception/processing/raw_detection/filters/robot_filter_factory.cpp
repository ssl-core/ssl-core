#include "perception/processing/raw_detection/filters/robot_filter_factory.h"

namespace perception {

std::unique_ptr<IRobotFilter> RobotFilter::Factory::make() const {
  return std::make_unique<RobotFilter>();
}

} // namespace perception

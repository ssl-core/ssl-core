#ifndef PERCEPTION_FILTER_ROBOT_FILTER_H
#define PERCEPTION_FILTER_ROBOT_FILTER_H

#include "perception/entities/raw_robot.h"

namespace perception {

class RobotFilter {
 public:
  explicit RobotFilter();

  void update(const RawRobot& robot);

  [[nodiscard]] RawRobot getRobot() const;

 private:
  // TODO(#ISSUE_N): Aggregate a FilteredRobot instead.
  RawRobot last_robot_;
};

} // namespace perception

#endif // PERCEPTION_FILTER_ROBOT_FILTER_H

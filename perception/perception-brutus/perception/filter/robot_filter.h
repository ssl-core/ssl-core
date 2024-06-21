#ifndef PERCEPTION_FILTER_ROBOT_FILTER_H
#define PERCEPTION_FILTER_ROBOT_FILTER_H

#include "perception/entities/raw_robot.h"

namespace perception {

/**
 * @class RobotFilter
 * @brief Filters raw robot detections.
 */
class RobotFilter {
 public:
  explicit RobotFilter(const RawRobot& robot);

  ~RobotFilter() = default;
  RobotFilter(const RobotFilter& other) = default;
  RobotFilter(RobotFilter&& other) = default;
  RobotFilter& operator=(const RobotFilter& other) = default;
  RobotFilter& operator=(RobotFilter&& other) = default;

  /**
   * @brief Updates the filter with a new raw robot detection.
   * @param robot The new raw robot detection.
   */
  void update(const RawRobot& robot);

  /**
   * @brief Gets the last robot detection processed by the filter.
   * @return The last raw robot detection.
   */
  [[nodiscard]] RawRobot getRobot() const;

 private:
  // TODO(#ISSUE_N): Aggregate a FilteredRobot instead.
  RawRobot last_robot_; /**< The last raw robot detection processed by the filter. */
};

} // namespace perception

#endif // PERCEPTION_FILTER_ROBOT_FILTER_H

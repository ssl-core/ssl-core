#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_ROBOT_FILTER_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_ROBOT_FILTER_H

#include "perception/processing/raw_detection/entities/raw_robot.h"

#include <optional>

namespace perception {

class IRobotFilter {
 public:
  class Factory;

  IRobotFilter() = default;

  IRobotFilter(const IRobotFilter& other) = delete;
  IRobotFilter& operator=(const IRobotFilter& other) = delete;
  IRobotFilter(IRobotFilter&& other) = default;
  IRobotFilter& operator=(IRobotFilter&& other) = default;

  virtual ~IRobotFilter() = default;

  virtual void update(const RawRobot& robot) = 0;
  [[nodiscard]] virtual std::optional<RawRobot> getRobot() const = 0;
};

class RobotFilter : public IRobotFilter {
 public:
  class Factory;

  RobotFilter() = default;

  void update(const RawRobot& robot) override;
  [[nodiscard]] std::optional<RawRobot> getRobot() const override;

 private:
  // TODO(matheusvtna): Aggregate a FilteredRobot instead.
  std::optional<RawRobot> last_robot_; /**< The last raw robot detection processed by the filter. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_ROBOT_FILTER_H

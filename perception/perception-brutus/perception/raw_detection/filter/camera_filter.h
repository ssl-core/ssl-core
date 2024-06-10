#ifndef PERCEPTION_FILTER_CAMERA_FILTER_H
#define PERCEPTION_FILTER_CAMERA_FILTER_H

#include "perception/raw_detection/filter/ball_filter.h"
#include "perception/raw_detection/filter/robot_filter.h"
#include "perception/raw_detection/raw_detection.h"

#include <cstdint>
#include <deque>
#include <protocols/perception/detection.pb.h>

namespace perception {

using protocols::perception::Detection;

/**
 * @class CameraFilter
 * @brief Filters raw detections from a single camera to produce a Detection message.
 */
class CameraFilter {
 public:
  explicit CameraFilter() = default;
  ~CameraFilter() = default;
  CameraFilter(const CameraFilter& other) = default;
  CameraFilter(CameraFilter&& other) = default;
  CameraFilter& operator=(const CameraFilter& other) = default;
  CameraFilter& operator=(CameraFilter&& other) = default;

  /**
   * @brief Gets the last detection produced by the filter.
   * @return The last Detection produced by the filter.
   */
  Detection getDetection() const;

  /**
   * @brief Filters a raw detection to produce a Detection message.
   * @param raw_detection The raw detection to filter.
   * @return The filtered Detection message.
   */
  Detection filter(const RawDetection& raw_detection);

 private:
  Detection last_detection_; /**< The last produced detection. */
  int64_t serial_id_{};      /**< The serial ID for detections. */
  std::map<RobotID, std::unique_ptr<RobotFilter>>
      robot_filters_;                   /**< Filters for individual robots. */
  std::deque<BallFilter> ball_filters_; /**< Filters for ball detections. */

  /**
   * @brief Filters raw robot detections.
   * @param robots The raw robot detections to filter.
   * @return The filtered robot detections.
   */
  std::vector<RawRobot> filterRobots(const std::vector<RawRobot>& robots);

  /**
   * @brief Filters raw ball detections.
   * @param balls The raw ball detections to filter.
   * @return The filtered ball detections.
   */
  std::vector<RawBall> filterBalls(const std::vector<RawBall>& balls);
};

} // namespace perception

#endif // PERCEPTION_FILTER_CAMERA_FILTER_H

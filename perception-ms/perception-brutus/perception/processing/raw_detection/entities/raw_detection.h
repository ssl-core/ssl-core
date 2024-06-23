#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H

#include "perception/processing/raw_detection/entities/raw_ball.h"
#include "perception/processing/raw_detection/entities/raw_robot.h"

#include <cstdint>
#include <span>
#include <vector>

namespace perception {

/**
 * @class RawDetection
 * @brief Represents the raw detection data including balls and robots detected in a camera frame.
 *
 * The RawDetection class encapsulates the information about detected balls and robots
 * in a given camera frame, including frame number and camera ID.
 */
class RawDetection {
 public:
  /**
   * @brief Default constructor for RawDetection.
   *
   * Initializes an empty RawDetection object.
   */
  RawDetection();

  /**
   * @brief Parameterized constructor for RawDetection.
   * @param frame_number The frame number associated with this detection.
   * @param camera_id The ID of the camera that captured this frame.
   * @param balls A vector of detected RawBall objects.
   */
  RawDetection(int64_t frame_number,
               int camera_id,
               std::vector<RawBall> balls,
               std::vector<RawRobot> robots);

  /**
   * @brief Gets the camera Id.
   * @return The ID of the camera that captured this frame.
   */
  [[nodiscard]] int getCameraId() const;

  /**
   * @brief Gets the frame number.
   * @return The frame number associated with this detection.
   */
  [[nodiscard]] int64_t getFrameNumber() const;

  /**
   * @brief Gets the detected balls.
   * @return A vector of detected RawBall objects.
   */
  [[nodiscard]] std::span<const RawBall> getBalls() const;

  /**
   * @brief Gets all detected robots (yellow and blue).
   * @return A vector of all detected RawRobot objects.
   */
  [[nodiscard]] std::span<const RawRobot> getRobots() const;

 private:
  int64_t frame_number_;         /**< The frame number associated with this detection. */
  int camera_id_;                /**< The ID of the camera that captured this frame. */
  std::vector<RawBall> balls_;   /**< List of detected balls in this frame. */
  std::vector<RawRobot> robots_; /**< List of detected robots in this frame. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H

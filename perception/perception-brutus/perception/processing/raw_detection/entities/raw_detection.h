#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H

#include "perception/processing/raw_detection/entities/raw_ball.h"
#include "perception/processing/raw_detection/entities/raw_robot.h"

#include <cstdint>
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
   * @param yellow_robots A vector of detected yellow RawRobot objects.
   * @param blue_robots A vector of detected blue RawRobot objects.
   */
  RawDetection(int64_t frame_number,
               int camera_id,
               const std::vector<RawBall>& balls,
               const std::vector<RawRobot>& yellow_robots,
               const std::vector<RawRobot>& blue_robots);

  ~RawDetection() = default;
  RawDetection(const RawDetection& other) = default;
  RawDetection(RawDetection&& other) = default;
  RawDetection& operator=(const RawDetection& other) = default;
  RawDetection& operator=(RawDetection&& other) = default;

  /**
   * @brief Gets the camera ID.
   * @return The ID of the camera that captured this frame.
   */
  [[nodiscard]] int getCameraID() const;

  /**
   * @brief Gets the frame number.
   * @return The frame number associated with this detection.
   */
  [[nodiscard]] int64_t getFrameNumber() const;

  /**
   * @brief Gets the detected balls.
   * @return A vector of detected RawBall objects.
   */
  [[nodiscard]] std::vector<RawBall> getBalls() const;

  /**
   * @brief Gets the detected yellow robots.
   * @return A vector of detected yellow RawRobot objects.
   */
  [[nodiscard]] std::vector<RawRobot> getYellowRobots() const;

  /**
   * @brief Gets the detected blue robots.
   * @return A vector of detected blue RawRobot objects.
   */
  [[nodiscard]] std::vector<RawRobot> getBlueRobots() const;

  /**
   * @brief Gets all detected robots (yellow and blue).
   * @return A vector of all detected RawRobot objects.
   */
  [[nodiscard]] std::vector<RawRobot> getRobots() const;

 private:
  int64_t frame_number_;                /**< The frame number associated with this detection. */
  int camera_id_;                       /**< The ID of the camera that captured this frame. */
  std::vector<RawBall> balls_;          /**< List of detected balls in this frame. */
  std::vector<RawRobot> yellow_robots_; /**< List of detected yellow robots in this frame. */
  std::vector<RawRobot> blue_robots_;   /**< List of detected blue robots in this frame. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_DETECTION_H

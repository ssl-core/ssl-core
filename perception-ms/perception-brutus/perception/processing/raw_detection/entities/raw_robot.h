#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H

#include "perception/processing/raw_detection/entities/robot_id.h"

#include <robocin/geometry/point2d.h>

namespace perception {

/**
 * @class RawRobot
 * @brief Represents a detected robot with confidence, id, position, and angle.
 *
 * The RawRobot class encapsulates the properties of a detected robot,
 * including its confidence level, id, 2D position, and orientation angle.
 */
class RawRobot {
 public:
  RawRobot() = default;

  /**
   * @brief Constructs a RawRobot with specified confidence, id, position, and angle.
   * @param confidence The confidence level of the robot detection.
   * @param id The RobotID of the detected robot.
   * @param position The 2D position of the detected robot.
   * @param angle The orientation angle of the detected robot.
   */
  RawRobot(float confidence, RobotId id, ::robocin::Point2Df position, float angle);

  /**
   * @brief Constructs a RawRobot with specified confidence, position, and angle.
   * @param confidence The confidence level of the robot detection.
   * @param position The 2D position of the detected robot.
   * @param angle The orientation angle of the detected robot.
   *
   * It creates a RawRobot object with an unknown id, defined with an id `-1` and
   * a Color `eColorUnspecified`.
   */
  RawRobot(float confidence, ::robocin::Point2Df position, float angle);

  /**
   * @brief Gets the confidence level of the robot detection.
   * @return The confidence level as a float.
   */
  [[nodiscard]] float getConfidence() const;

  /**
   * @brief Gets the id of the detected robot.
   * @return The id as a RobotID object.
   */
  [[nodiscard]] RobotId getRobotId() const;

  /**
   * @brief Gets the 2D position of the detected robot.
   * @return The position as a Point2D object.
   */
  [[nodiscard]] ::robocin::Point2Df getPosition() const;

  /**
   * @brief Gets the orientation angle of the detected robot.
   * @return The angle as a float.
   */
  [[nodiscard]] float getAngle() const;

 private:
  float confidence_{};             /**< Confidence level of the robot detection. */
  RobotId robot_id_{};             /**< id of the detected robot. */
  ::robocin::Point2Df position_{}; /**< 2D position of the detected robot. */
  float angle_{};                  /**< Orientation angle of the detected robot. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H

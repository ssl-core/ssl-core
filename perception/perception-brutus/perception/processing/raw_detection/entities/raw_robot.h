#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H

#include "perception/processing/raw_detection/entities/point.h"
#include "perception/processing/raw_detection/entities/robot_id.h"

namespace perception {

/**
 * @class RawRobot
 * @brief Represents a detected robot with confidence, ID, position, and angle.
 *
 * The RawRobot class encapsulates the properties of a detected robot,
 * including its confidence level, ID, 2D position, and orientation angle.
 */
class RawRobot {
 public:
  /**
   * @brief Constructs a RawRobot with specified confidence, ID, position, and angle.
   * @param confidence The confidence level of the robot detection.
   * @param id The RobotID of the detected robot.
   * @param position The 2D position of the detected robot.
   * @param angle The orientation angle of the detected robot.
   */
  RawRobot(float confidence, RobotID id, const Point2D& position, float angle);

  /**
   * @brief Constructs a RawRobot with specified confidence, position, and angle.
   * @param confidence The confidence level of the robot detection.
   * @param position The 2D position of the detected robot.
   * @param angle The orientation angle of the detected robot.
   *
   * It creates a RawRobot object with an unknown ID, defined with an ID `-1` and
   * a Color `eColorUnspecified`.
   */
  RawRobot(float confidence, const Point2D& position, float angle);

  ~RawRobot() = default;
  RawRobot(const RawRobot& other) = default;
  RawRobot(RawRobot&& other) = default;
  RawRobot& operator=(const RawRobot& other) = default;
  RawRobot& operator=(RawRobot&& other) = default;

  /**
   * @brief Gets the confidence level of the robot detection.
   * @return The confidence level as a float.
   */
  [[nodiscard]] float getConfidence() const;

  /**
   * @brief Gets the ID of the detected robot.
   * @return The ID as a RobotID object.
   */
  [[nodiscard]] RobotID getID() const;

  /**
   * @brief Gets the 2D position of the detected robot.
   * @return The position as a Point2D object.
   */
  [[nodiscard]] Point2D getPosition() const;

  /**
   * @brief Gets the orientation angle of the detected robot.
   * @return The angle as a float.
   */
  [[nodiscard]] float getAngle() const;

 private:
  float confidence_; /**< Confidence level of the robot detection. */
  RobotID id_;       /**< ID of the detected robot. */
  Point2D position_; /**< 2D position of the detected robot. */
  float angle_;      /**< Orientation angle of the detected robot. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_ROBOT_H

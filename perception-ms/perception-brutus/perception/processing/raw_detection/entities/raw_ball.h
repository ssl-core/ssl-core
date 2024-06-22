#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H

#include <robocin/geometry/point3d.h>

namespace perception {

/**
 * @class RawBall
 * @brief Represents a detected ball with confidence and position.
 *
 * The RawBall class encapsulates the properties of a detected ball,
 * including its confidence level and 3D position.
 */
class RawBall {
 public:
  RawBall() = default;

  /**
   * @brief Constructs a RawBall with specified confidence and position.
   * @param confidence The confidence level of the ball detection.
   * @param position The 3D position of the detected ball.
   */
  RawBall(float confidence, ::robocin::Point3Df position);

  /**
   * @brief Gets the confidence level of the ball detection.
   * @return The confidence level as a float.
   */
  [[nodiscard]] float getConfidence() const;

  /**
   * @brief Gets the 3D position of the detected ball.
   * @return The position as a Point3D object.
   */
  [[nodiscard]] ::robocin::Point3Df getPosition() const;

 private:
  float confidence_{};             /**< Confidence level of the ball detection. */
  ::robocin::Point3Df position_{}; /**< 3D position of the detected ball. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H

#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H

#include "perception/processing/raw_detection/entities/point.h"

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
  /**
   * @brief Constructs a RawBall with specified confidence and position.
   * @param confidence The confidence level of the ball detection.
   * @param position The 3D position of the detected ball.
   */
  RawBall(float confidence, const Point3D& position);

  ~RawBall() = default;
  RawBall(const RawBall& other) = default;
  RawBall(RawBall&& other) = default;
  RawBall& operator=(const RawBall& other) = default;
  RawBall& operator=(RawBall&& other) = default;

  /**
   * @brief Gets the confidence level of the ball detection.
   * @return The confidence level as a float.
   */
  [[nodiscard]] float getConfidence() const;

  /**
   * @brief Gets the 3D position of the detected ball.
   * @return The position as a Point3D object.
   */
  [[nodiscard]] Point3D getPosition() const;

 private:
  float confidence_; /**< Confidence level of the ball detection. */
  Point3D position_; /**< 3D position of the detected ball. */
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_RAW_BALL_H

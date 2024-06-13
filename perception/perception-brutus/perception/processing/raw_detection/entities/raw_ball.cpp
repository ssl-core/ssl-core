#include "perception/processing/raw_detection/entities/raw_ball.h"

namespace perception {

RawBall::RawBall(float confidence, const Point3D& position) :
    confidence_(confidence),
    position_(position) {}

float RawBall::getConfidence() const { return confidence_; }

Point3D RawBall::getPosition() const { return position_; }

} // namespace perception

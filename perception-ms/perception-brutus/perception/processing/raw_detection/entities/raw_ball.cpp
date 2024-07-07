#include "perception/processing/raw_detection/entities/raw_ball.h"

#include <robocin/geometry/point3d.h>

namespace perception {
namespace {

using ::robocin::Point3Df;

} // namespace

RawBall::RawBall(float confidence, Point3Df position) :
    confidence_{confidence},
    position_{position} {}

float RawBall::getConfidence() const { return confidence_; }

Point3Df RawBall::getPosition() const { return position_; }

} // namespace perception

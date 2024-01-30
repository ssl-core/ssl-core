// Ball.cpp
#include "vision/src/ball/ball.h"

#include <ostream>

Ball::Ball(std::string uuid,
           const Source& source,
           const Point2Df& position,
           const Point3Df& velocity) :
    uuid_(std::move(uuid)),
    source_(source),
    position_(position),
    velocity_(velocity) {}

std::ostream& operator<<(std::ostream& os, const Ball& ball) {
  os << "Ball(uuid=" << ball.uuid_ << ", source=" << ball.source_ << ", position=" << ball.position_
     << ", velocity=" << ball.velocity_ << ")";
  return os;
}

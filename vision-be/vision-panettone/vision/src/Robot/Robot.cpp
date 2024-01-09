// Robot.cpp
#include "Robot.h"

Robot::Robot(std::string uuid,
             int id,
             const Color& color,
             const Point2Df& position,
             double angle,
             const Point2Df& velocity,
             double angular_velocity,
             const Source& source) :
    uuid_(std::move(uuid)),
    id_(id),
    color_(color),
    position_(position),
    angle_(angle),
    velocity_(velocity),
    angular_velocity_(angular_velocity),
    source_(source) {}

std::ostream& operator<<(std::ostream& os, const Robot& robot) {
  os << "Robot(uuid=" << robot.uuid_ << ", id=" << robot.id_ << ", color=" << robot.color_
     << ", position=" << robot.position_ << ", angle=" << robot.angle_
     << ", velocity=" << robot.velocity_ << ", angular_velocity=" << robot.angular_velocity_
     << ", source=" << robot.source_ << ")";
  return os;
}

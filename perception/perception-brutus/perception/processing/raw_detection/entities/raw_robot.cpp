#include "perception/processing/raw_detection/entities/raw_robot.h"

namespace perception {

RawRobot::RawRobot(float confidence, RobotID id, const Point2D& position, float angle) :
    confidence_(confidence),
    id_(id),
    position_(position),
    angle_(angle) {}

float RawRobot::getConfidence() const { return confidence_; }

RobotID RawRobot::getID() const { return id_; }

Point2D RawRobot::getPosition() const { return position_; }

float RawRobot::getAngle() const { return angle_; }

} // namespace perception

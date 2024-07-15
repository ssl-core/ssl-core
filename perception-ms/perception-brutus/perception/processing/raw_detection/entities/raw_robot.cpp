#include "perception/processing/raw_detection/entities/raw_robot.h"

#include <robocin/geometry/point2d.h>

namespace perception {
namespace {

using ::robocin::Point2Df;

} // namespace

RawRobot::RawRobot(float confidence, RobotId robot_id, Point2Df position, float angle) :
    confidence_{confidence},
    robot_id_{robot_id},
    position_{position},
    angle_{angle} {}

float RawRobot::getConfidence() const { return confidence_; }

RobotId RawRobot::getRobotId() const { return robot_id_; }

Point2Df RawRobot::getPosition() const { return position_; }

float RawRobot::getAngle() const { return angle_; }

} // namespace perception

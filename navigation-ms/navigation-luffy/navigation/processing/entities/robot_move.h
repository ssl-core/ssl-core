#ifndef NAVIGATION_PROCESSING_ROBOT_MOVE_H
#define NAVIGATION_PROCESSING_ROBOT_MOVE_H

#include <robocin/geometry/point2d.h>

namespace navigation {

class RobotMove {
  ::robocin::Point2Dd velocity_;
  double angularVelocity_;

 public:
  RobotMove(::robocin::Point2Dd velocity, double angularVelocity) :
      velocity_(velocity),
      angularVelocity_(angularVelocity) {}

  explicit RobotMove(double angularVelocity) :
      velocity_(::robocin::Point2Dd(0.0, 0.0)),
      angularVelocity_(angularVelocity) {}

  RobotMove() : velocity_(::robocin::Point2Dd(0.0, 0.0)), angularVelocity_(0.0) {}

  [[nodiscard]] ::robocin::Point2Dd velocity() const { return velocity_; }
  [[nodiscard]] double angularVelocity() const { return angularVelocity_; }
};

} // namespace navigation

#endif // NAVIGATION_PROCESSING_ROBOT_MOVE_H

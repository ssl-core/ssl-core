#include "perception/tracked_frame/converter/tracked_frame_converter.h"

namespace perception {

Detection TrackedFrameConverter::convertTrackedFrame(const TrackedFrame& tracked_frame) {
  Detection detection;
  const uint32_t k60_fps = 60;
  detection.set_frame_rate(k60_fps);

  for (const auto& tracked_robot : tracked_frame.robots()) {
    Robot& robot = *detection.add_robots();
    robot = convertTrackedRobot(tracked_robot);
  }

  for (const auto& tracked_ball : tracked_frame.balls()) {
    Ball& ball = *detection.add_balls();
    ball = convertTrackedBall(tracked_ball);
  }

  return detection;
}

Robot TrackedFrameConverter::convertTrackedRobot(const TrackedRobot& tracked_robot) {
  Robot robot;

  robot.set_confidence(tracked_robot.visibility()); // Assuming visibility means confidence...
  robot.mutable_robot_id()->CopyFrom(tracked_robot.robot_id());

  protocols::common::Point2Df* position = robot.mutable_position();
  position->set_x(tracked_robot.pos().x());
  position->set_y(tracked_robot.pos().y());

  robot.set_angle(tracked_robot.orientation());

  protocols::common::Point2Df* velocity = robot.mutable_velocity();
  if (tracked_robot.has_vel()) {
    velocity->set_x(tracked_robot.vel().x());
    velocity->set_y(tracked_robot.vel().y());
  }

  robot.set_angular_velocity(tracked_robot.has_vel_angular() ? tracked_robot.vel_angular() : 0);

  robot.set_radius(0);
  robot.set_height(0);
  robot.set_dribbler_width(0);

  return robot;
}

Ball TrackedFrameConverter::convertTrackedBall(const TrackedBall& tracked_ball) {
  Ball ball;

  ball.set_confidence(tracked_ball.visibility());

  protocols::common::Point3Df* position = ball.mutable_position();
  position->set_x(tracked_ball.pos().x());
  position->set_y(tracked_ball.pos().y());
  position->set_z(tracked_ball.pos().z());

  protocols::common::Point3Df* velocity = ball.mutable_velocity();
  if (tracked_ball.has_vel()) {
    velocity->set_x(tracked_ball.vel().x());
    velocity->set_y(tracked_ball.vel().y());
    velocity->set_z(tracked_ball.vel().z());
  }

  return ball;
}

} // namespace perception

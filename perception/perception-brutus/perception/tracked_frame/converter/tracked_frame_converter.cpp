#include "perception/tracked_frame/converter/tracked_frame_converter.h"

#include <chrono>
#include <google/protobuf/timestamp.pb.h>
#include <protocols/common/robot_id.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/common.pb.h>

namespace perception {

using google::protobuf::Timestamp;

Detection TrackedFrameConverter::convertTrackedFrame(const TrackedFrame& tracked_frame) {
  Detection detection;

  const int64_t kNanosPerSecond = 1000000000;
  Timestamp* timestamp = detection.mutable_created_at();
  auto now = std::chrono::system_clock::now();
  std::time_t seconds = std::chrono::system_clock::to_time_t(now);
  auto duration = now.time_since_epoch();
  auto nanos
      = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % kNanosPerSecond;
  timestamp->set_seconds(static_cast<int64_t>(seconds));
  timestamp->set_nanos(static_cast<int32_t>(nanos));

  detection.set_serial_id(tracked_frame.frame_number());

  const uint32_t k60_fps = 60;
  detection.set_framerate(k60_fps);

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
  protocols::common::RobotId& robot_id = *robot.mutable_robot_id();
  robot_id.set_number(tracked_robot.robot_id().id());

  auto mapped_color = [&] {
    using protocols::third_party::game_controller::Team;
    using protocols::common::RobotId;
    switch (tracked_robot.robot_id().team()) {
      case Team::BLUE: return RobotId::COLOR_BLUE;
      case Team::YELLOW: return RobotId::COLOR_YELLOW;
      default: return RobotId::COLOR_UNSPECIFIED;
    }
  }();
  robot_id.set_color(mapped_color);

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

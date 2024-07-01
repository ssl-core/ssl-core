#include "perception/processing/tracked_detection/mappers/tracked_detection_mapper.h"

#include "perception/parameters/parameters.h"

#include <google/protobuf/timestamp.pb.h>
#include <protocols/common/robot_id.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/common.pb.h>
#include <utility>

namespace perception {
namespace {

using ::google::protobuf::Timestamp;
using ::robocin::IPbTimeUtil;

namespace rc {

using ::protocols::common::Point2Df;
using ::protocols::common::Point3Df;
using ::protocols::common::RobotId;
using ::protocols::perception::Ball;
using ::protocols::perception::Detection;
using ::protocols::perception::Robot;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Team;
using ::protocols::third_party::game_controller::TrackedBall;
using ::protocols::third_party::game_controller::TrackedFrame;
using ::protocols::third_party::game_controller::TrackedRobot;
using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

constexpr float kMetersToMillimeters = 1000.0F;

rc::Robot robotFromTrackedRobot(const tp::TrackedRobot& tracked_robot) {
  rc::Robot robot;

  // TODO(matheusvtna): assuming visibility means confidence...
  robot.set_confidence(tracked_robot.visibility());

  auto color_mapper = [](tp::Team team) {
    switch (team) {
      case tp::Team::BLUE: return rc::RobotId::COLOR_BLUE;
      case tp::Team::YELLOW: return rc::RobotId::COLOR_YELLOW;
      default: return rc::RobotId::COLOR_UNSPECIFIED;
    }

    std::unreachable();
  };

  rc::RobotId& robot_id = *robot.mutable_robot_id();
  robot_id.set_number(tracked_robot.robot_id().id());
  robot_id.set_color(color_mapper(tracked_robot.robot_id().team()));

  rc::Point2Df& position = *robot.mutable_position();
  position.set_x(tracked_robot.pos().x() * kMetersToMillimeters);
  position.set_y(tracked_robot.pos().y() * kMetersToMillimeters);

  robot.set_angle(tracked_robot.orientation());

  if (rc::Point2Df& velocity = *robot.mutable_velocity(); tracked_robot.has_vel()) {
    velocity.set_x(tracked_robot.vel().x() * kMetersToMillimeters);
    velocity.set_y(tracked_robot.vel().y() * kMetersToMillimeters);
  }

  if (tracked_robot.has_vel_angular()) {
    robot.set_angular_velocity(tracked_robot.vel_angular());
  }

  robot.set_radius(pRobotRadius());
  robot.set_height(pRobotHeight());
  robot.set_dribbler_width(pDribblerWidth());

  return robot;
}

rc::Ball ballFromTrackedBall(const tp::TrackedBall& tracked_ball) {
  rc::Ball ball;

  ball.set_confidence(tracked_ball.visibility());

  rc::Point3Df& position = *ball.mutable_position();
  position.set_x(tracked_ball.pos().x() * kMetersToMillimeters);
  position.set_y(tracked_ball.pos().y() * kMetersToMillimeters);
  position.set_z(tracked_ball.pos().z() * kMetersToMillimeters);

  if (rc::Point3Df& velocity = *ball.mutable_velocity(); tracked_ball.has_vel()) {
    velocity.set_x(tracked_ball.vel().x() * kMetersToMillimeters);
    velocity.set_y(tracked_ball.vel().y() * kMetersToMillimeters);
    velocity.set_z(tracked_ball.vel().z() * kMetersToMillimeters);
  }

  return ball;
}

} // namespace

TrackedDetectionMapper::TrackedDetectionMapper(std::unique_ptr<IPbTimeUtil> pb_time_util) :
    pb_time_util_{std::move(pb_time_util)} {}

rc::Detection TrackedDetectionMapper::fromTrackedWrapperPacket(
    const tp::TrackerWrapperPacket& tracked_wrapper_packet) {
  const tp::TrackedFrame& tracked_frame = tracked_wrapper_packet.tracked_frame();

  rc::Detection detection;

  Timestamp& timestamp = *detection.mutable_created_at();
  timestamp = pb_time_util_->getCurrentTime();

  for (const auto& tracked_robot : tracked_frame.robots()) {
    rc::Robot& robot = *detection.add_robots();
    robot = robotFromTrackedRobot(tracked_robot);
  }

  for (const auto& tracked_ball : tracked_frame.balls()) {
    rc::Ball& ball = *detection.add_balls();
    ball = ballFromTrackedBall(tracked_ball);
  }

  return detection;
}

} // namespace perception

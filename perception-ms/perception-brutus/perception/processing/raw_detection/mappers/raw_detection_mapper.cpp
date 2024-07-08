#include "perception/processing/raw_detection/mappers/raw_detection_mapper.h"

#include <robocin/geometry/point3d.h>

namespace perception {

namespace {

using ::robocin::Point2Df;
using ::robocin::Point3Df;

namespace tp {

using ::protocols::third_party::detection::SSL_DetectionBall;
using ::protocols::third_party::detection::SSL_DetectionFrame;
using ::protocols::third_party::detection::SSL_DetectionRobot;
using ::protocols::third_party::detection::SSL_WrapperPacket;

} // namespace tp

RawRobot fromSslDetectionRobot(const tp::SSL_DetectionRobot& robot, RobotId::Color color) {
  return RawRobot{
      robot.confidence(),
      RobotId{static_cast<int>(robot.robot_id()), color},
      Point2Df{robot.x(), robot.y()},
      robot.orientation(),
  };
}

RawBall fromSslDetectionBall(const tp::SSL_DetectionBall& ball) {
  return RawBall{
      ball.confidence(),
      Point3Df{
          ball.x(),
          ball.y(),
          ball.z(),
      },
  };
}

} // namespace

RawDetection
RawDetectionMapper::fromSslWrapperPacket(const tp::SSL_WrapperPacket& wrapper_packet) const {
  const tp::SSL_DetectionFrame& detection_frame = wrapper_packet.detection();

  std::vector<RawBall> balls;
  std::vector<RawRobot> robots;

  for (const auto& ball : detection_frame.balls()) {
    balls.emplace_back(fromSslDetectionBall(ball));
  }

  for (const auto& robot : detection_frame.robots_blue()) {
    robots.emplace_back(fromSslDetectionRobot(robot, RobotId::Color::Blue));
  }

  for (const auto& robot : detection_frame.robots_yellow()) {
    robots.emplace_back(fromSslDetectionRobot(robot, RobotId::Color::Yellow));
  }

  return RawDetection{
      detection_frame.frame_number(),
      static_cast<int>(detection_frame.camera_id()),
      std::move(balls),
      std::move(robots),
  };
}

} // namespace perception

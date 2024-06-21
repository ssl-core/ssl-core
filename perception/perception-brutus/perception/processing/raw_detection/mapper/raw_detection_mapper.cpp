#include "perception/processing/raw_detection/mapper/raw_detection_mapper.h"

namespace perception {

RawDetection
RawDetectionConverter::convertDetectionFrame(const SSL_DetectionFrame& detection_frame) {
  std::vector<RawBall> balls;
  std::vector<RawRobot> blues;
  std::vector<RawRobot> yellows;

  for (const auto& robot : detection_frame.robots_blue()) {
    blues.push_back(convertRobot(robot, RobotColor::eColorBlue));
  }

  for (const auto& robot : detection_frame.robots_yellow()) {
    yellows.push_back(convertRobot(robot, RobotColor::eColorYellow));
  }

  for (const auto& ball : detection_frame.balls()) {
    balls.push_back(convertBall(ball));
  }

  return {detection_frame.frame_number(),
          static_cast<int>(detection_frame.camera_id()),
          balls,
          yellows,
          blues};
}

RawRobot RawDetectionConverter::convertRobot(const SSL_DetectionRobot& robot, RobotColor color) {
  return RawRobot(robot.confidence(),
                  RobotID{robot.robot_id(), color},
                  Point2D{robot.x(), robot.y()},
                  robot.orientation());
}

RawBall RawDetectionConverter::convertBall(const SSL_DetectionBall& ball) {
  return {ball.confidence(), Point3D{ball.x(), ball.y(), ball.z()}};
}

} // namespace perception

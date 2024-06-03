#include "perception/filter/camera_filter.h"

namespace perception {

using protocols::common::RobotId_Color;
using protocols::perception::Ball;
using protocols::perception::Robot;

Detection CameraFilter::getDetection() const { return last_detection_; }

Detection CameraFilter::filter(const RawDetection& raw_detection) {
  auto filtered_robots = filterRobots(raw_detection.getRobots());
  auto filtered_balls = filterBalls(raw_detection.getBalls());

  Detection detection;
  detection.set_serial_id(serial_id_++);

  auto map_color = [](RobotColor color) {
    if (color == RobotColor::eColorBlue) {
      return RobotId_Color::RobotId_Color_COLOR_BLUE;
    }
    if (color == RobotColor::eColorBlue) {
      return RobotId_Color::RobotId_Color_COLOR_YELLOW;
    }
    return RobotId_Color::RobotId_Color_COLOR_UNSPECIFIED;
  };

  for (const auto& filtered_robot : filtered_robots) {
    Robot& detection_robot = *detection.add_robots();
    detection_robot.mutable_robot_id()->set_number(filtered_robot.getID().id);
    detection_robot.mutable_robot_id()->set_color(map_color(filtered_robot.getID().color));
    detection_robot.mutable_position()->set_x(filtered_robot.getPosition().x);
    detection_robot.mutable_position()->set_y(filtered_robot.getPosition().y);
  }

  for (const auto& filtered_ball : filtered_balls) {
    Ball& detection_ball = *detection.add_balls();
    detection_ball.mutable_position()->set_x(filtered_ball.getPosition().x);
    detection_ball.mutable_position()->set_y(filtered_ball.getPosition().y);
  }

  return detection;
}

std::vector<RawRobot> CameraFilter::filterRobots(const std::vector<RawRobot>& robots) {
  for (const auto& robot : robots) {
    auto robot_id = robot.getID();
    if (robots_filter_.find(robot_id) == robots_filter_.end()) {
      robots_filter_[robot_id] = RobotFilter();
    }
    robots_filter_[robot.getID()].update(robot);
  }

  std::vector<RawRobot> filtered_robots;
  filtered_robots.reserve(robots_filter_.size());
  for (const auto& [_, filter] : robots_filter_) {
    filtered_robots.push_back(filter.getRobot());
  }
  return filtered_robots;
}

std::vector<RawBall> CameraFilter::filterBalls(const std::vector<RawBall>& raw_balls) {

  for (const auto& ball : raw_balls) {
    bool ball_filter_found = false;
    for (auto& ball_filter : balls_filter_) {
      if (ball_filter.update(ball)) {
        ball_filter_found = true;
        break;
      }
    }
  }

  std::vector<RawBall> filtered_balls;
  filtered_balls.reserve(balls_filter_.size());
  for (const auto& ball_filter : balls_filter_) {
    filtered_balls.push_back(ball_filter.getBall());
  }
  return filtered_balls;
}

} // namespace perception

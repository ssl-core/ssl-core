#include "perception/filter/camera_filter.h"

#include "ball_filter.h"
#include "robot_filter.h"

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

  last_detection_ = detection;
  return detection;
}

std::vector<RawRobot> CameraFilter::filterRobots(const std::vector<RawRobot>& robots) {
  for (const auto& robot : robots) {
    auto robot_id = robot.getID();
    if (robot_filters_.find(robot_id) == robot_filters_.end()) {
      auto new_robot_filter = RobotFilter(robot);
      robot_filters_[robot_id] = std::make_unique<RobotFilter>(new_robot_filter);
    } else {
      robot_filters_[robot_id]->update(robot);
    }
  }

  std::vector<RawRobot> filtered_robots;
  filtered_robots.reserve(robot_filters_.size());
  for (const auto& [_, filter] : robot_filters_) {
    filtered_robots.push_back(filter->getRobot());
  }
  return filtered_robots;
}

std::vector<RawBall> CameraFilter::filterBalls(const std::vector<RawBall>& raw_balls) {

  for (const auto& ball : raw_balls) {
    bool ball_filter_found = false;
    for (auto& ball_filter : ball_filters_) {
      if (ball_filter.update(ball)) {
        ball_filter_found = true;
        break;
      }
    }

    if (not ball_filter_found) {
      BallFilter new_ball_filter(ball);
      ball_filters_.push_back(new_ball_filter);
    }
  }

  std::vector<RawBall> filtered_balls;
  filtered_balls.reserve(ball_filters_.size());
  for (const auto& ball_filter : ball_filters_) {
    filtered_balls.push_back(ball_filter.getBall());
  }
  return filtered_balls;
}

} // namespace perception

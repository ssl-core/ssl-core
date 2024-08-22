#include "perception/processing/raw_detection/filters/camera_filter.h"

#include "perception/processing/raw_detection/entities/raw_ball.h"
#include "perception/processing/raw_detection/entities/raw_robot.h"
#include "perception/processing/raw_detection/filters/ball_filter.h"
#include "perception/processing/raw_detection/filters/ball_filter_factory.h"
#include "perception/processing/raw_detection/filters/robot_filter.h"
#include "perception/processing/raw_detection/filters/robot_filter_factory.h"

#include <protocols/common/robot_id.pb.h>
#include <protocols/perception/detection.pb.h>

namespace perception {
namespace {

namespace rc {

using ::protocols::common::RobotId_Color;
using ::protocols::perception::Ball;
using ::protocols::perception::Detection;
using ::protocols::perception::Robot;

} // namespace rc

} // namespace

CameraFilter::CameraFilter(std::unique_ptr<IBallFilter::Factory> ball_filter_factory,
                           std::unique_ptr<IRobotFilter::Factory> robot_filter_factory) :
    ball_filter_factory_{std::move(ball_filter_factory)},
    robot_filter_factory_{std::move(robot_filter_factory)} {}

void CameraFilter::update(const RawDetection& raw_detection) {
  std::vector<RawBall> filtered_balls = filterBalls(raw_detection.getBalls());
  std::vector<RawRobot> filtered_robots = filterRobots(raw_detection.getRobots());

  rc::Detection detection;

  for (const auto& filtered_ball : filtered_balls) {
    rc::Ball& detection_ball = *detection.add_balls();

    detection_ball.mutable_position()->set_x(filtered_ball.getPosition().x);
    detection_ball.mutable_position()->set_y(filtered_ball.getPosition().y);
  }

  // TODO(matheusvtna, joseviccruz): replace lines below with mappers.
  auto color_mapper = [](RobotId::Color color) {
    switch (color) {
      case RobotId::Color::Yellow: return rc::RobotId_Color::RobotId_Color_COLOR_YELLOW;
      case RobotId::Color::Blue: return rc::RobotId_Color::RobotId_Color_COLOR_BLUE;
      default: return rc::RobotId_Color::RobotId_Color_COLOR_UNSPECIFIED;
    };

    std::unreachable();
  };

  for (const auto& filtered_robot : filtered_robots) {
    rc::Robot& detection_robot = *detection.add_robots();

    detection_robot.mutable_robot_id()->set_number(filtered_robot.getRobotId().id);
    detection_robot.mutable_robot_id()->set_color(color_mapper(filtered_robot.getRobotId().color));
    detection_robot.mutable_position()->set_x(filtered_robot.getPosition().x);
    detection_robot.mutable_position()->set_y(filtered_robot.getPosition().y);
  }

  last_detection_ = std::move(detection);
}

std::optional<rc::Detection> CameraFilter::getDetection() const { return last_detection_; }

std::vector<RawBall> CameraFilter::filterBalls(std::span<const RawBall> raw_balls) {
  for (const RawBall& raw_ball : raw_balls) {
    bool ball_filter_updated = false;

    for (auto& ball_filter : ball_filters_) {
      if (ball_filter->update(raw_ball)) {
        ball_filter_updated = true;
        break;
      }
    }

    if (not ball_filter_updated) {
      // TODO(matheusvtna, joseviccruz): should we remove ball filter when expire?
      std::unique_ptr<IBallFilter> ball_filter = ball_filter_factory_->make();
      ball_filter->update(raw_ball);

      ball_filters_.emplace_back(std::move(ball_filter));
    }
  }

  std::vector<RawBall> filtered_balls;
  filtered_balls.reserve(ball_filters_.size());

  for (const auto& ball_filter : ball_filters_) {
    if (std::optional<RawBall> raw_ball = ball_filter->getBall()) {
      filtered_balls.emplace_back(std::move(*raw_ball)); // NOLINT(*move*)
    }
  }

  return filtered_balls;
}

std::vector<RawRobot> CameraFilter::filterRobots(std::span<const RawRobot> raw_robots) {
  for (const RawRobot& raw_robot : raw_robots) {
    RobotId robot_id = raw_robot.getRobotId();
    auto it = robot_filters_.find(robot_id);

    if (it == robot_filters_.end()) {
      std::tie(it, std::ignore) = robot_filters_.emplace(robot_id, robot_filter_factory_->make());
    }

    it->second->update(raw_robot);
  }

  std::vector<RawRobot> filtered_robots;
  filtered_robots.reserve(robot_filters_.size());

  for (const auto& [_, filter] : robot_filters_) {
    if (std::optional<RawRobot> raw_robot = filter->getRobot()) {
      filtered_robots.emplace_back(std::move(*raw_robot)); // NOLINT(*move*)
    }
  }

  return filtered_robots;
}

} // namespace perception

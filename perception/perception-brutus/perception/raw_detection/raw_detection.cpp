#include "perception/raw_detection/raw_detection.h"

#include <cstdint>
#include <vector>

namespace perception {

RawDetection::RawDetection(int64_t frame_number,
                           int camera_id,
                           const std::vector<RawBall>& balls,
                           const std::vector<RawRobot>& yellow_robots,
                           const std::vector<RawRobot>& blue_robots) :

    frame_number_(frame_number),
    camera_id_(camera_id),
    balls_(balls),
    yellow_robots_(yellow_robots),
    blue_robots_(blue_robots) {}

int RawDetection::getCameraID() const { return camera_id_; }

int64_t RawDetection::getFrameNumber() const { return frame_number_; }

std::vector<RawBall> RawDetection::getBalls() const { return balls_; }

std::vector<RawRobot> RawDetection::getYellowRobots() const { return yellow_robots_; }

std::vector<RawRobot> RawDetection::getBlueRobots() const { return blue_robots_; }

std::vector<RawRobot> RawDetection::getRobots() const {
  std::vector<RawRobot> robots;
  robots.reserve(yellow_robots_.size() + blue_robots_.size());
  robots.insert(robots.end(), yellow_robots_.begin(), yellow_robots_.end());
  robots.insert(robots.end(), blue_robots_.begin(), blue_robots_.end());
  return robots;
}

} // namespace perception

#include "perception/processing/raw_detection/entities/raw_detection.h"

#include <cstdint>
#include <utility>
#include <vector>

namespace perception {

RawDetection::RawDetection(int64_t frame_number, // NOLINT(*swappable*)
                           int camera_id,
                           std::vector<RawBall> balls,
                           std::vector<RawRobot> robots) :
    frame_number_{frame_number},
    camera_id_{camera_id},
    balls_{std::move(balls)},
    robots_{std::move(robots)} {}

int RawDetection::getCameraId() const { return camera_id_; }

int64_t RawDetection::getFrameNumber() const { return frame_number_; }

std::span<const RawBall> RawDetection::getBalls() const { return balls_; }

std::span<const RawRobot> RawDetection::getRobots() const { return robots_; }

} // namespace perception

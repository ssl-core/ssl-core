#ifndef PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_H
#define PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_H

#include "perception/processing/raw_detection/entities/raw_detection.h"
#include "perception/processing/raw_detection/filters/ball_filter.h"
#include "perception/processing/raw_detection/filters/robot_filter.h"

#include <deque>
#include <map>
#include <protocols/perception/detection.pb.h>

namespace perception {

class ICameraFilter {
 public:
  class Factory;

  ICameraFilter() = default;

  ICameraFilter(const ICameraFilter& other) = delete;
  ICameraFilter(ICameraFilter&& other) = delete;
  ICameraFilter& operator=(const ICameraFilter& other) = default;
  ICameraFilter& operator=(ICameraFilter&& other) = default;

  virtual ~ICameraFilter() = default;

  virtual void update(const RawDetection& raw_detection) = 0;
  [[nodiscard]] virtual ::protocols::perception::Detection getDetection() const = 0;
};

class CameraFilter : public ICameraFilter {
 public:
  class Factory;

  CameraFilter(std::unique_ptr<IBallFilter::Factory> ball_filter_factory,
               std::unique_ptr<IRobotFilter::Factory> robot_filter_factory);

  void update(const RawDetection& raw_detection) override;
  [[nodiscard]] ::protocols::perception::Detection getDetection() const override;

 private:
  std::vector<RawBall> filterBalls(std::span<const RawBall> raw_balls);
  std::vector<RawRobot> filterRobots(std::span<const RawRobot> raw_robots);

  ::protocols::perception::Detection last_detection_;
  // TODO(joseviccruz): replace by absl::flat_hash_map
  std::unique_ptr<IBallFilter::Factory> ball_filter_factory_;
  std::unique_ptr<IRobotFilter::Factory> robot_filter_factory_;
  std::map<RobotId, std::unique_ptr<IRobotFilter>> robot_filters_;
  std::deque<std::unique_ptr<IBallFilter>> ball_filters_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_H

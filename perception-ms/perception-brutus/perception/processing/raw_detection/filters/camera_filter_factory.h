#ifndef PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_FACTORY_H
#define PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_FACTORY_H

#include "perception/processing/raw_detection/filters/ball_filter.h"
#include "perception/processing/raw_detection/filters/camera_filter.h"

#include <functional>

namespace perception {

class ICameraFilter::Factory {
 public:
  Factory() = default;

  Factory(const Factory& other) = delete;
  Factory(Factory&& other) = delete;
  Factory& operator=(const Factory& other) = default;
  Factory& operator=(Factory&& other) = default;

  virtual ~Factory() = default;

  [[nodiscard]] virtual std::unique_ptr<ICameraFilter> make() const = 0;
};

class CameraFilter::Factory : public ICameraFilter::Factory {
 public:
  Factory(
      std::move_only_function<std::unique_ptr<IBallFilter::Factory>()> ball_filter_factory_fn,
      std::move_only_function<std::unique_ptr<IRobotFilter::Factory>()> robot_filter_factory_fn);

  [[nodiscard]] std::unique_ptr<ICameraFilter> make() const override;

 private:
  mutable std::move_only_function<std::unique_ptr<IBallFilter::Factory>()> ball_filter_factory_fn_;
  mutable std::move_only_function<std::unique_ptr<IRobotFilter::Factory>()>
      robot_filter_factory_fn_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_FILTER_CAMERA_FILTER_FACTORY_H

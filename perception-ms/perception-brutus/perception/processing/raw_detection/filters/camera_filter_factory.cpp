#include "perception/processing/raw_detection/filters/camera_filter_factory.h"

#include "perception/processing/raw_detection/filters/ball_filter_factory.h"
#include "perception/processing/raw_detection/filters/robot_filter_factory.h"

namespace perception {

CameraFilter::Factory::Factory(
    std::move_only_function<std::unique_ptr<IBallFilter::Factory>() const> ball_filter_factory_fn,
    std::move_only_function<std::unique_ptr<IRobotFilter::Factory>() const>
        robot_filter_factory_fn) :
    ball_filter_factory_fn_{std::move(ball_filter_factory_fn)},
    robot_filter_factory_fn_{std::move(robot_filter_factory_fn)} {}

std::unique_ptr<ICameraFilter> CameraFilter::Factory::make() const {
  return std::make_unique<CameraFilter>(ball_filter_factory_fn_(), robot_filter_factory_fn_());
}

} // namespace perception

#include "perception/processing/raw_detection/filters/ball_filter_factory.h"

namespace perception {

std::unique_ptr<IBallFilter> BallFilter::Factory::make() const {
  return std::make_unique<BallFilter>();
}

} // namespace perception

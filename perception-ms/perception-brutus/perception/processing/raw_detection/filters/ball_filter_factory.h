#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_FACTORY_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_FACTORY_H

#include "perception/processing/raw_detection/filters/ball_filter.h"

#include <memory>

namespace perception {

class IBallFilter::Factory {
 public:
  Factory() = default;

  Factory(const Factory& other) = delete;
  Factory& operator=(const Factory& other) = delete;
  Factory(Factory&& other) = default;
  Factory& operator=(Factory&& other) = default;

  virtual ~Factory() = default;

  [[nodiscard]] virtual std::unique_ptr<IBallFilter> make() const = 0;
};

class BallFilter::Factory : public IBallFilter::Factory {
 public:
  Factory() = default;

  [[nodiscard]] std::unique_ptr<IBallFilter> make() const override;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_FACTORY_H

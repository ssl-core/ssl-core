#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_H

#include "perception/processing/raw_detection/entities/raw_ball.h"

#include <optional>

namespace perception {

class IBallFilter {
 public:
  class Factory;

  IBallFilter() = default;

  IBallFilter(const IBallFilter& other) = delete;
  IBallFilter& operator=(const IBallFilter& other) = delete;
  IBallFilter(IBallFilter&& other) = default;
  IBallFilter& operator=(IBallFilter&& other) = default;

  virtual ~IBallFilter() = default;

  virtual bool update(const RawBall& raw_ball) = 0;
  [[nodiscard]] virtual std::optional<RawBall> getBall() const = 0;
};

/**
 * @class BallFilter
 * @brief Filters raw ball data to produce filtered ball information.
 *
 * The BallFilter class is responsible for filtering raw ball data to provide
 * more accurate and reliable ball information.
 */
class BallFilter : public IBallFilter {
 public:
  class Factory;

  BallFilter() = default;

  bool update(const RawBall& raw_ball) override;
  [[nodiscard]] std::optional<RawBall> getBall() const override;

 private:
  // TODO(matheusvtna): Aggregate a FilteredBall instead.
  std::optional<RawBall> last_ball_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_BALL_FILTER_H

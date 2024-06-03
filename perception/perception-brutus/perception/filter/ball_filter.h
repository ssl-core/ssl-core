#ifndef PERCEPTION_FILTER_BALL_FILTER_H
#define PERCEPTION_FILTER_BALL_FILTER_H

#include "perception/entities/raw_ball.h"

namespace perception {

/**
 * @class BallFilter
 * @brief Filters raw ball data to produce filtered ball information.
 *
 * The BallFilter class is responsible for filtering raw ball data to provide
 * more accurate and reliable ball information.
 */
class BallFilter {
 public:
  explicit BallFilter();

  /**
   * @brief Updates the ball filter with new raw ball data.
   * @param raw_ball The raw ball data to update the filter.
   * @return True if the filter was successfully updated, false otherwise.
   */
  bool update(const RawBall& raw_ball);

  /**
   * @brief Retrieves the filtered ball information.
   * @return The filtered ball information.
   */
  [[nodiscard]] RawBall getBall() const;

 private:
  // TODO(#ISSUE_N): Aggregate a FilteredBall instead.
  RawBall last_ball_; /**< The last filtered ball information. */
};

} // namespace perception

#endif // PERCEPTION_FILTER_BALL_FILTER_H

#include "perception/filter/ball_filter.h"

namespace perception {

bool BallFilter::update(const RawBall& raw_ball) {
  // TODO(#ISSUE_N): Implement the ball filter update to determine if the raw_ball should be
  // accepted as the same ball it was tracking before.
  last_ball_ = raw_ball;
  return true;
}

RawBall BallFilter::getBall() const { return last_ball_; }

} // namespace perception

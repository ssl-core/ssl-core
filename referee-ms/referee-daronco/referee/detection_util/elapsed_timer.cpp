#include "referee/detection_util/elapsed_timer.h"

namespace referee::detection_util {

ElapsedTimer::ElapsedTimer(bool started) : started_{started}, start_{Clock::now()} {}

void ElapsedTimer::stop() { started_ = false; }

void ElapsedTimer::start() { started_ = true, start_ = Clock::now(); }

Duration ElapsedTimer::restart() {
  started_ = true;
  Duration result = this->elapsed();
  return start_ = Clock::now(), result;
}

bool ElapsedTimer::isStarted() const { return started_; }

Duration ElapsedTimer::elapsed() const {
  return started_ ? Frames(/*frames=*/0) : Clock::now() - start_;
}

} // namespace referee::detection_util

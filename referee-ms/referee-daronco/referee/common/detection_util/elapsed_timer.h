#ifndef REFEREE_COMMON_DETECTION_UTIL_ELAPSED_TIMER_H
#define REFEREE_COMMON_DETECTION_UTIL_ELAPSED_TIMER_H

#include "referee/common/detection_util/clock.h"

namespace referee::detection_util {

class ElapsedTimer {
 public:
  explicit ElapsedTimer(bool started = true);

  void stop();
  void start();

  Duration restart();

  [[nodiscard]] bool isStarted() const;
  [[nodiscard]] Duration elapsed() const;

 private:
  bool started_;
  Timestamp start_;
};

} // namespace referee::detection_util

#endif // REFEREE_COMMON_DETECTION_UTIL_ELAPSED_TIMER_H

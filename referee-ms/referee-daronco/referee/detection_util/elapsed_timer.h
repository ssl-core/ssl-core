#ifndef REFEREE_DETECTION_ELAPSED_TIMER_H
#define REFEREE_DETECTION_ELAPSED_TIMER_H

#include "referee/detection_util/clock.h"
#include "referee/detection_util/duration.h"
#include "referee/detection_util/timestamp.h"

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

#endif // REFEREE_DETECTION_ELAPSED_TIMER_H

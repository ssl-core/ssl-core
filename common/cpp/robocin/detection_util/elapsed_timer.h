#ifndef ROBOCIN_DETECTION_UTIL_ELAPSED_TIMER_H
#define ROBOCIN_DETECTION_UTIL_ELAPSED_TIMER_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/detection_util/timestamp.h"

namespace robocin::detection_util {

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

} // namespace robocin::detection_util

#endif

#endif // ROBOCIN_DETECTION_UTIL_ELAPSED_TIMER_H

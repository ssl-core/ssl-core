#ifndef ROBOCIN_DETECTION_UTIL_CLOCK_H
#define ROBOCIN_DETECTION_UTIL_CLOCK_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/detection_util/duration.h"
#include "robocin/detection_util/timestamp.h"

#include <cstdint>
#include <memory>

namespace robocin::detection_util {

class IClockEngine;

class Clock {
 public:
  using timestamp_type = Timestamp;
  using duration_type = Duration;

  Clock() = delete;

  static void set_clock(std::unique_ptr<IClockEngine> engine); // NOLINT(*naming*)

  static uint32_t framerate();
  static timestamp_type now();
};

class IClockEngine {
 public:
  IClockEngine() = default;

  IClockEngine(const IClockEngine&) = delete;
  IClockEngine& operator=(const IClockEngine&) = delete;
  IClockEngine(IClockEngine&&) noexcept = default;
  IClockEngine& operator=(IClockEngine&&) noexcept = default;

  virtual ~IClockEngine() = default;

  virtual uint32_t framerate() = 0;
  virtual Clock::timestamp_type now() = 0;

  // Caution: To maintain clock consistency, update should not be called by multiple threads.
  virtual void update(uint32_t framerate, Clock::timestamp_type timestamp) = 0;
};

class ClockEngine : public IClockEngine {
 public:
  ClockEngine() = default;

  uint32_t framerate() override;
  Clock::timestamp_type now() override;

  void update(uint32_t framerate, Clock::timestamp_type timestamp) override;
};

} // namespace robocin::detection_util

#endif

#endif // ROBOCIN_DETECTION_UTIL_CLOCK_H

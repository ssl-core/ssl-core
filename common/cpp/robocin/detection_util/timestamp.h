#ifndef ROBOCIN_DETECTION_UTIL_TIMESTAMP_H
#define ROBOCIN_DETECTION_UTIL_TIMESTAMP_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/detection_util/duration.h"

#include <cstdint>

namespace robocin::detection_util {

class Timestamp {
 public:
  Timestamp(uint64_t serial_id, int64_t seconds, int32_t nanos);

  [[nodiscard]] uint64_t serial_id() const; // NOLINT(*naming*)
  [[nodiscard]] int64_t seconds() const;
  [[nodiscard]] int32_t nanos() const;

  Duration operator-(const Timestamp& other) const;

 private:
  uint64_t serial_id_;
  int64_t seconds_;
  int32_t nanos_;
};

} // namespace robocin::detection_util

#endif

#endif // ROBOCIN_DETECTION_UTIL_TIMESTAMP_H

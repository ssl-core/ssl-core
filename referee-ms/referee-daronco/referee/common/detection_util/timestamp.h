#ifndef REFEREE_COMMON_DETECTION_UTIL_TIMESTAMP_H
#define REFEREE_COMMON_DETECTION_UTIL_TIMESTAMP_H

#include "referee/common/detection_util/duration.h"

#include <cstdint>

namespace referee::detection_util {

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

} // namespace referee::detection_util

#endif // REFEREE_COMMON_DETECTION_UTIL_TIMESTAMP_H

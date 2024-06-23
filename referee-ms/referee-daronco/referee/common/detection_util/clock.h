#ifndef REFEREE_COMMON_DETECTION_UTIL_CLOCK_H
#define REFEREE_COMMON_DETECTION_UTIL_CLOCK_H

#include "referee/common/detection_util/duration.h"
#include "referee/common/detection_util/timestamp.h"

#include <cstdint>
#include <protocols/perception/detection.pb.h>

namespace referee::detection_util {

class Clock {
 public:
  Clock() = delete;

  using timestamp_type = Timestamp;
  using duration_type = Duration;

  static timestamp_type now();
  static uint32_t framerate();

  // Caution: To maintain clock consistency, update should not be called by multiple threads.
  static void update(const ::protocols::perception::Detection& detection);
};

} // namespace referee::detection_util

#endif // REFEREE_COMMON_DETECTION_UTIL_CLOCK_H

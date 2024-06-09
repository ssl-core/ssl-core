#ifndef REFEREE_DETECTION_CLOCK_H
#define REFEREE_DETECTION_CLOCK_H

#include "referee/detection_util/duration.h"
#include "referee/detection_util/timestamp.h"

#include <cstdint>
#include <protocols/perception/detection.pb.h>

namespace referee::detection_util {

class Timestamp;
class Duration;

class Clock {
 public:
  using timestamp_type = Timestamp;
  using duration_type = Duration;

  static timestamp_type now();
  static uint32_t framerate();

  // Caution: To maintain clock consistency, update should not be called by multiple threads.
  static void update(const ::protocols::perception::Detection& detection);
};

} // namespace referee::detection_util

#endif // REFEREE_DETECTION_CLOCK_H

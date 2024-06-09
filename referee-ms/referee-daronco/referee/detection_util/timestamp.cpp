#include "referee/detection_util/timestamp.h"

namespace referee::detection_util {

Timestamp::Timestamp(uint64_t serial_id, // NOLINT(*swappable*)
                     int64_t seconds,
                     int32_t nanos) :
    serial_id_(serial_id),
    seconds_(seconds),
    nanos_(nanos) {}

uint64_t Timestamp::serial_id() const { return serial_id_; }

int64_t Timestamp::seconds() const { return seconds_; }

int32_t Timestamp::nanos() const { return nanos_; }

Duration Timestamp::operator-(const Timestamp& other) const {
  return Frames(static_cast<int64_t>(serial_id_) - static_cast<int64_t>(other.serial_id_));
}

} // namespace referee::detection_util

#include "mock/common/utility.h"

namespace ines {
namespace {

constexpr int64_t kNanosPerSecond = 1'000'000'000L;

} // namespace

google::protobuf::Timestamp timestampFromNanos(int64_t nanos) {
  google::protobuf::Timestamp timestamp;
  timestamp.set_seconds(nanos / kNanosPerSecond);
  timestamp.set_nanos(static_cast<int32_t>(nanos % kNanosPerSecond));
  return timestamp;
}

google::protobuf::Timestamp timestampFromTime(const absl::Time &time) {
  google::protobuf::Timestamp timestamp;
  timestamp.set_seconds(absl::ToUnixSeconds(time));
  timestamp.set_nanos(
      static_cast<int32_t>(absl::ToUnixNanos(time) % kNanosPerSecond));
  return timestamp;
}

absl::Time timeFromTimestamp(const google::protobuf::Timestamp &timestamp) {
  return absl::FromUnixNanos(timestamp.seconds() * kNanosPerSecond +
                             timestamp.nanos());
}

} // namespace ines

#include "robocin/third_party/adaptors/pb_time_util.h"

#include <absl/time/clock.h>

namespace robocin {

::google::protobuf::Timestamp PbTimeUtil::getCurrentTime() {
  return ::google::protobuf::util::TimeUtil::NanosecondsToTimestamp(absl::GetCurrentTimeNanos());
}

} // namespace robocin

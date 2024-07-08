#include "robocin/third_party/adaptors/pb_time_util.h"

namespace robocin {

::google::protobuf::Timestamp PbTimeUtil::getCurrentTime() {
  return ::google::protobuf::util::TimeUtil::GetCurrentTime();
}

} // namespace robocin

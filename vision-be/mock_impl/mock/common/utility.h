#ifndef INES_COMMON_UTILITY_H
#define INES_COMMON_UTILITY_H

#include <absl/time/time.h>

#include <google/protobuf/timestamp.pb.h>

namespace ines {

::google::protobuf::Timestamp timestampFromNanos(int64_t nanos);

::google::protobuf::Timestamp timestampFromTime(const ::absl::Time& time);

::absl::Time timeFromTimestamp(const ::google::protobuf::Timestamp& timestamp);

} // namespace ines

#endif // INES_COMMON_UTILITY_H

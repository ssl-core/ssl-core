#ifndef ROBOCIN_THIRD_PARTY_ADAPTORS_PB_TIME_UTIL_H
#define ROBOCIN_THIRD_PARTY_ADAPTORS_PB_TIME_UTIL_H

#include <google/protobuf/util/time_util.h>

namespace robocin {

class IPbTimeUtil {
 public:
  IPbTimeUtil() = default;

  IPbTimeUtil(const IPbTimeUtil&) = delete;
  IPbTimeUtil& operator=(const IPbTimeUtil&) = delete;
  IPbTimeUtil(IPbTimeUtil&&) = default;
  IPbTimeUtil& operator=(IPbTimeUtil&&) = default;

  virtual ~IPbTimeUtil() = default;

  virtual ::google::protobuf::Timestamp getCurrentTime() = 0;
};

class PbTimeUtil : public IPbTimeUtil {
 public:
  PbTimeUtil() = default;

  ::google::protobuf::Timestamp getCurrentTime() override;
};

} // namespace robocin

#endif // ROBOCIN_THIRD_PARTY_ADAPTORS_PB_TIME_UTIL_H

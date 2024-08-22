#include "robocin/detection_util/duration.h"

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include "robocin/detection_util/clock.h"

#include <cstdint>

namespace robocin::detection_util {
namespace {

constexpr int64_t kMillisecondsPerSecond = 1000LL;
constexpr int64_t kSecondsPerMinute = 60;

} // namespace

int64_t Duration::ms() const { return kMillisecondsPerSecond * frames_ / Clock::framerate(); }

Duration Milliseconds(int64_t milliseconds) noexcept {
  return Duration{(Clock::framerate() * milliseconds) / kMillisecondsPerSecond};
}

Duration Seconds(int64_t seconds) noexcept { return Duration{Clock::framerate() * seconds}; }

Duration Minutes(int64_t minutes) noexcept {
  return Duration{Clock::framerate() * kSecondsPerMinute * minutes};
}

inline namespace duration_literals {

Duration operator"" _ms(unsigned long long milliseconds) noexcept {
  return Milliseconds(static_cast<int64_t>(milliseconds));
}

Duration operator"" _s(unsigned long long seconds) noexcept {
  return Seconds(static_cast<int64_t>(seconds));
}

Duration operator"" _min(unsigned long long minutes) noexcept {
  return Minutes(static_cast<int64_t>(minutes));
}

} // namespace duration_literals

} // namespace robocin::detection_util

#endif

#include "referee/detection_util/duration.h"

#include "referee/detection_util/clock.h"

#include <cstdint>

namespace referee::detection_util {
namespace {

constexpr int64_t kMillisPerSecond = 1000LL;
constexpr int64_t kSecondsPerMinute = 60;

} // namespace

int64_t Duration::ms() const { return kMillisPerSecond * frames_ / Clock::framerate(); }
int64_t Duration::frames() const { return frames_; }

Duration& Duration::operator+=(const Duration& other) { return frames_ += other.frames_, *this; }
Duration& Duration::operator-=(const Duration& other) { return frames_ -= other.frames_, *this; }

Duration Duration::operator+(const Duration& other) const { return Duration{*this} += other; }
Duration Duration::operator-(const Duration& other) const { return Duration{*this} -= other; }

Duration& Duration::operator*=(int64_t factor) { return frames_ *= factor, *this; }
Duration& Duration::operator/=(int64_t factor) { return frames_ /= factor, *this; }

Duration Duration::operator*(int64_t factor) const { return Duration{*this} *= factor; }
Duration Duration::operator/(int64_t factor) const { return Duration{*this} /= factor; }

Duration Duration::operator+() const { return Duration{*this}; }
Duration Duration::operator-() const { return Duration{-frames_}; }

Duration Milliseconds(int64_t milliseconds) noexcept {
  return Duration{(Clock::framerate() * milliseconds) / kMillisPerSecond};
}

Duration Seconds(int64_t seconds) noexcept { return Duration{Clock::framerate() * seconds}; }

Duration Minutes(int64_t minutes) noexcept {
  return Duration{Clock::framerate() * kSecondsPerMinute * minutes};
}

Duration Frames(int64_t frames) noexcept { return Duration{frames}; }

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

Duration operator"" _frames(unsigned long long frames) noexcept {
  return Frames(static_cast<int64_t>(frames));
}

} // namespace duration_literals

} // namespace referee::detection_util

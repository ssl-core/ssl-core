#ifndef ROBOCIN_DETECTION_UTIL_DURATION_H
#define ROBOCIN_DETECTION_UTIL_DURATION_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

#include <compare> // IWYU pragma: keep
#include <cstdint>

namespace robocin::detection_util {

class Duration {
 public:
  constexpr Duration() = default;

  [[nodiscard]] int64_t ms() const;
  [[nodiscard]] constexpr int64_t frames() const { return frames_; }

  constexpr Duration& operator+=(const Duration& other) { return frames_ += other.frames_, *this; }
  constexpr Duration& operator-=(const Duration& other) { return frames_ -= other.frames_, *this; }

  constexpr Duration operator+(const Duration& other) const { return Duration{*this} += other; }
  constexpr Duration operator-(const Duration& other) const { return Duration{*this} -= other; }

  constexpr Duration& operator*=(int64_t factor) { return frames_ *= factor, *this; }
  constexpr Duration& operator/=(int64_t factor) { return frames_ /= factor, *this; }

  constexpr Duration operator*(int64_t factor) const { return Duration{*this} *= factor; }
  constexpr Duration operator/(int64_t factor) const { return Duration{*this} /= factor; }

  constexpr Duration operator+() const { return Duration{*this}; }
  constexpr Duration operator-() const { return Duration{-frames_}; }

  constexpr friend bool operator==(const Duration& lhs, const Duration& rhs) = default;
  constexpr friend auto operator<=>(const Duration& lhs, const Duration& rhs) = default;

  // NOLINTBEGIN(*naming*)
  friend Duration Milliseconds(int64_t milliseconds) noexcept;
  friend Duration Seconds(int64_t seconds) noexcept;
  friend Duration Minutes(int64_t minutes) noexcept;
  friend constexpr Duration Frames(int64_t frames) noexcept;
  // NOLINTEND(*naming*)

 private:
  constexpr explicit Duration(int64_t frames) : frames_{frames} {}

  int64_t frames_{0};
};

// Caution: When using a duration in milliseconds, make sure that it is sufficiently longer than the
// generation time of a new detection because the duration in perception is discrete, for example:
//
// Let it be a framerate of 60 frames per second. The update time of a new frame, in milliseconds,
// will be 1000 / 60 ~ 16ms, so any value below 16ms will be considered 0.
//
// Tip: If you need a short enough duration, use "Frames" and control the time at frame level.
Duration Milliseconds(int64_t milliseconds) noexcept;
Duration Seconds(int64_t seconds) noexcept;
Duration Minutes(int64_t minutes) noexcept;
constexpr Duration Frames(int64_t frames) noexcept { return Duration{frames}; }

inline namespace duration_literals {

Duration operator"" _ms(unsigned long long milliseconds) noexcept;
Duration operator"" _s(unsigned long long seconds) noexcept;
Duration operator"" _min(unsigned long long minutes) noexcept;
constexpr Duration operator"" _frames(unsigned long long frames) noexcept {
  return Frames(static_cast<int64_t>(frames));
}

} // namespace duration_literals

} // namespace robocin::detection_util

#endif

#endif // ROBOCIN_DETECTION_UTIL_DURATION_H

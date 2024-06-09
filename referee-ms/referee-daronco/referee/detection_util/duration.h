#ifndef REFEREE_COMMON_DETECTION_DURATION_H
#define REFEREE_COMMON_DETECTION_DURATION_H

#include <compare> // IWYU pragma: keep
#include <cstdint>

namespace referee::detection_util {

class Duration {
 public:
  Duration() = default;

  [[nodiscard]] int64_t ms() const;
  [[nodiscard]] int64_t frames() const;

  Duration& operator+=(const Duration& other);
  Duration& operator-=(const Duration& other);
  Duration operator+(const Duration& other) const;
  Duration operator-(const Duration& other) const;

  Duration& operator*=(int64_t factor);
  Duration& operator/=(int64_t factor);
  Duration operator*(int64_t factor) const;
  Duration operator/(int64_t factor) const;

  Duration operator+() const;
  Duration operator-() const;

  friend bool operator==(const Duration& lhs, const Duration& rhs) = default;
  friend auto operator<=>(const Duration& lhs, const Duration& rhs) = default;

  // NOLINTBEGIN(*naming*)
  friend Duration Milliseconds(int64_t milliseconds) noexcept;
  friend Duration Seconds(int64_t seconds) noexcept;
  friend Duration Minutes(int64_t minutes) noexcept;
  friend Duration Frames(int64_t frames) noexcept;
  // NOLINTEND(*naming*)

 private:
  explicit Duration(int64_t frames);

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
Duration Frames(int64_t frames) noexcept;

inline namespace duration_literals {

Duration operator"" _ms(unsigned long long milliseconds) noexcept;
Duration operator"" _s(unsigned long long seconds) noexcept;
Duration operator"" _min(unsigned long long minutes) noexcept;
Duration operator"" _frames(unsigned long long frames) noexcept;

} // namespace duration_literals

} // namespace referee::detection_util

#endif // REFEREE_COMMON_DETECTION_DURATION_H

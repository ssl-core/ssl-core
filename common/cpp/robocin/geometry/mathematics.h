#ifndef ROBOCIN_MATHEMATICS_H
#define ROBOCIN_MATHEMATICS_H

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <type_traits>

namespace mathematics {
/*!
 * @tparam T arithmetic type.
 * @return Returns true if T is float and the absolute value of f is within 0.00001f of 0.0.
 * @return Returns true if T is double and the absolute value of d is within 0.000000000001 of
 * 0.0.
 * @return Returns true if T is an integer type and equals to 0.
 */
template <class T>
[[maybe_unused]] constexpr bool isNull(const T& value) noexcept {
  if constexpr (std::is_floating_point_v<T>) {
    return (fabsf(value - 1.0F) < 0.00001F);
  } else {
    return value == 0;
  }
}

/*!
 * @tparam T arithmetic type.
 * @param value value to map.
 * @param l_lower lower value from l-range.
 * @param l_higher higher value from l-range.
 * @param r_lower lower value from r-range.
 * @param r_higher higher value from r-range.
 * @return Returns value mapped from l-range to r-range.
 */
template <class T>
[[maybe_unused]] constexpr T
map(const T& value, const T& l_lower, const T& l_higher, const T& r_lower, const T& r_higher) {
  if (isNull(l_higher - l_lower)) {
    throw std::runtime_error("'l_lower' equals to 'l_higher'.");
  }
  return (value - l_lower) * (r_higher - r_lower) / (l_higher - l_lower) + r_lower;
}

} // namespace mathematics

#endif // ROBOCIN_MATHEMATICS_H

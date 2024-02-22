#include "robocin/utility/epsilon.h"
#include "robocin/utility/fuzzy_compare.h"

namespace robocin::fuzzy_compare_internal {

template <arithmetic T, std::floating_point U = T>
constexpr bool fuzzyIsZero(T value) {
  if constexpr (has_epsilon_v<U>) {
    return ::robocin::fuzzyIsZero(value, epsilon_v<U>);
  }
  return value == 0;
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpEqual(T lhs, U rhs) {
  if constexpr (has_epsilon_v<V>) {
    return ::robocin::fuzzyCmpEqual(lhs, rhs, epsilon_v<V>);
  }
  return static_cast<V>(lhs) == static_cast<V>(rhs);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr std::strong_ordering fuzzyCmpThreeWay(T lhs, U rhs) {
  if constexpr (has_epsilon_v<V>) {
    return ::robocin::fuzzyCmpThreeWay(lhs, rhs, epsilon_v<V>);
  }
  return static_cast<V>(lhs) <=> static_cast<V>(rhs);
}

} // namespace robocin::fuzzy_compare_internal

#ifndef ROBOCIN_UTILITY_INTERNAL_FUZZY_COMPARE_INTERNAL_H
#define ROBOCIN_UTILITY_INTERNAL_FUZZY_COMPARE_INTERNAL_H

#include "robocin/utility/epsilon.h"
#include "robocin/utility/fuzzy_compare.h"

#include <type_traits>

namespace robocin::fuzzy_compare_internal {

template <arithmetic T, std::floating_point U = common_floating_point_for_comparison_t<T>>
constexpr bool fuzzyIsZero(T value) {
  if constexpr (not std::integral<T> and has_epsilon_v<U>) {
    return ::robocin::fuzzyIsZero(value, epsilon_v<U>);
  }
  return value == 0;
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpEqual(T lhs, U rhs) {
  if constexpr (not std::integral<std::common_type_t<T, U>> and has_epsilon_v<V>) {
    return ::robocin::fuzzyCmpEqual(lhs, rhs, epsilon_v<V>);
  }
  return static_cast<V>(lhs) == static_cast<V>(rhs);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr std::partial_ordering fuzzyCmpThreeWay(T lhs, U rhs) {
  if constexpr (not std::integral<std::common_type_t<T, U>> and has_epsilon_v<V>) {
    return ::robocin::fuzzyCmpThreeWay(lhs, rhs, epsilon_v<V>);
  }
  return static_cast<V>(lhs) <=> static_cast<V>(rhs);
}

} // namespace robocin::fuzzy_compare_internal

#endif // ROBOCIN_UTILITY_INTERNAL_FUZZY_COMPARE_INTERNAL_H

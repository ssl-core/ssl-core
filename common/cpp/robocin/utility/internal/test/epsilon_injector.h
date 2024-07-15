// This file is used to inject the epsilon values into the epsilon struct if they are not already
// defined. It should be included in the test files, but not in the library files.

#ifndef ROBOCIN_UTILITY_INTERNAL_TEST_EPSILON_INJECTOR_H
#define ROBOCIN_UTILITY_INTERNAL_TEST_EPSILON_INJECTOR_H

#include "robocin/utility/epsilon.h"

namespace robocin {

#if not defined(ROBOCIN_FLOAT_EPSILON)
template <>
struct has_epsilon<float> : std::true_type {
  static constexpr float epsilon = 1e-2F; // NOLINT(readability-identifier-naming)
};
#endif

#if not defined(ROBOCIN_DOUBLE_EPSILON)
template <>
struct has_epsilon<double> : std::true_type {
  static constexpr double epsilon = 1e-4; // NOLINT(readability-identifier-naming)
};
#endif

#if not defined(ROBOCIN_LONG_DOUBLE_EPSILON)
template <>
struct has_epsilon<long double> : std::true_type {
  static constexpr long double epsilon = 1e-6L; // NOLINT(readability-identifier-naming)
};
#endif

} // namespace robocin

#endif // ROBOCIN_UTILITY_INTERNAL_TEST_EPSILON_INJECTOR_H

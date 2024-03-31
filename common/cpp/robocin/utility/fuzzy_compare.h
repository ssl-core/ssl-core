#ifndef ROBOCIN_UTILITY_FUZZY_COMPARE_H
#define ROBOCIN_UTILITY_FUZZY_COMPARE_H

#include "robocin/utility/concepts.h"
#include "robocin/utility/epsilon.h"
#include "robocin/utility/type_traits.h"

#include <cmath>
#include <compare>
#include <concepts>

namespace robocin {

template <arithmetic T, std::floating_point U>
constexpr bool fuzzyIsZero(T value, U epsilon) {
  return std::abs(value) <= epsilon;
}

template <arithmetic T, std::floating_point U = T>
constexpr bool fuzzyIsZero(T value)
  requires(has_epsilon_v<U>)
{
  return fuzzyIsZero(value, epsilon_v<U>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpEqual(T lhs, U rhs, V epsilon) {
  return std::abs(lhs - rhs) <= epsilon;
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpEqual(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpEqual(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpNotEqual(T lhs, U rhs, V epsilon) {
  return not fuzzyCmpEqual(lhs, rhs, epsilon);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpNotEqual(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return not fuzzyCmpEqual(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr std::partial_ordering fuzzyCmpThreeWay(T lhs, U rhs, V epsilon) {
  if (fuzzyCmpEqual(lhs, rhs, epsilon)) {
    return std::partial_ordering::equivalent;
  }
  return (lhs < rhs) ? std::partial_ordering::less : std::partial_ordering::greater;
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr std::partial_ordering fuzzyCmpThreeWay(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpThreeWay(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpLess(T lhs, U rhs, V epsilon) {
  return std::is_lt(fuzzyCmpThreeWay(lhs, rhs, epsilon));
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpLess(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpLess(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpLessEqual(T lhs, U rhs, V epsilon) {
  return std::is_lteq(fuzzyCmpThreeWay(lhs, rhs, epsilon));
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpLessEqual(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpLessEqual(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpGreater(T lhs, U rhs, V epsilon) {
  return std::is_gt(fuzzyCmpThreeWay(lhs, rhs, epsilon));
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpGreater(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpGreater(lhs, rhs, epsilon_v<V>);
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpGreaterEqual(T lhs, U rhs, V epsilon) {
  return std::is_gteq(fuzzyCmpThreeWay(lhs, rhs, epsilon));
}

template <arithmetic T,
          arithmetic U,
          std::floating_point V = common_floating_point_for_comparison_t<T, U>>
constexpr bool fuzzyCmpGreaterEqual(T lhs, U rhs)
  requires(has_epsilon_v<V>)
{
  return fuzzyCmpGreaterEqual(lhs, rhs, epsilon_v<V>);
}

template <std::floating_point F>
class FuzzyIsZero {
 public:
  using value_type = F;

  constexpr FuzzyIsZero()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyIsZero(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type value) const { return fuzzyIsZero(value, epsilon_); }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyEqualTo {
 public:
  using value_type = F;

  constexpr FuzzyEqualTo()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyEqualTo(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpEqual(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyNotEqualTo {
 public:
  using value_type = F;

  constexpr FuzzyNotEqualTo()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyNotEqualTo(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpNotEqual(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyThreeWay {
 public:
  using value_type = F;

  constexpr FuzzyThreeWay()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyThreeWay(value_type epsilon) : epsilon_{epsilon} {}

  constexpr std::partial_ordering operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpThreeWay(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyLess {
 public:
  using value_type = F;

  constexpr FuzzyLess()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyLess(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpLess(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyLessEqual {
 public:
  using value_type = F;

  constexpr FuzzyLessEqual()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyLessEqual(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpLessEqual(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyGreater {
 public:
  using value_type = F;

  constexpr FuzzyGreater()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyGreater(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpGreater(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

template <std::floating_point F>
class FuzzyGreaterEqual {
 public:
  using value_type = F;

  constexpr FuzzyGreaterEqual()
    requires(has_epsilon_v<value_type>)
      : epsilon_{epsilon_v<value_type>} {}

  constexpr explicit FuzzyGreaterEqual(value_type epsilon) : epsilon_{epsilon} {}

  constexpr bool operator()(value_type lhs, value_type rhs) const {
    return fuzzyCmpGreaterEqual(lhs, rhs, epsilon_);
  }

 private:
  value_type epsilon_;
};

} // namespace robocin

#endif // ROBOCIN_UTILITY_FUZZY_COMPARE_H

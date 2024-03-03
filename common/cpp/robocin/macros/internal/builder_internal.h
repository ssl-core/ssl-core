#ifndef ROBOCIN_MACROS_INTERNAL_BUILDER_INTERNAL_H
#define ROBOCIN_MACROS_INTERNAL_BUILDER_INTERNAL_H

#include "robocin/container/span.h"
#include "robocin/utility/concepts.h"

#include <string>
#include <string_view>
#include <utility>

namespace robocin::builder_internal {

// getter-related ----------------------------------------------------------------------------------

template <class T>
struct getter_result { // NOLINT(*naming*)
  using type = const T&;
};

template <fundamentalish T>
struct getter_result<T> {
  using type = T;
};

template <class T>
struct getter_result<std::basic_string<T>> {
  using type = std::basic_string_view<T>;
};

template <class T>
  requires(std::constructible_from<Span<const typename T::value_type>, T>)
struct getter_result<T> {
  using type = Span<const typename T::value_type>;
};

template <optionalish T>
struct getter_result<T> : getter_result<typename T::value_type> {};

template <class T>
using getter_result_t = typename getter_result<T>::type;

template <class T>
struct get_optional_result { // NOLINT(*naming*)
  using type = const T&;
};

template <class T>
using get_optional_result_t = typename get_optional_result<T>::type;

// optional-related --------------------------------------------------------------------------------

template <auto Value>
struct is_compile_time_evaluable { // NOLINT(*naming*)
  using type = decltype(Value);
};

template <optionalish Optional>
getter_result_t<typename Optional::value_type>
get_or_default(const Optional& opt) { // NOLINT(*naming*)
  if (opt) {
    return *opt;
  }

  using T = typename Optional::value_type;

  if constexpr (requires { typename is_compile_time_evaluable<T{}>::type; }) {
    static constexpr T kResult{};
    return kResult;
  } else {
    static const T kResult{};
    return kResult;
  }
}

template <optionalish T>
bool has_value(const T& opt) { // NOLINT(*naming*)
  return opt.has_value();
}

// container-related -------------------------------------------------------------------------------

template <class Container>
concept pushable = requires(Container& container) {
  requires (
    requires { container.push_back(std::declval<typename Container::value_type>()); } ||
    requires { container.insert(std::declval<typename Container::value_type>()); } ||
    requires { container.insert(container.end(), std::declval<typename Container::value_type>()); }
  );
};

template <class Container, class T>
void push_to(Container& container, T&& value) { // NOLINT(*naming*)
  // if has push back, use it (vector, deque, list...)
  if constexpr (requires { container.push_back(std::forward<T>(value)); }) {
    container.push_back(std::forward<T>(value));
  }
  // else if has insert without hint, use it (set, map, unordered_set, unordered_map...)
  else if constexpr (requires { container.insert(std::forward<T>(value)); }) {
    container.insert(std::forward<T>(value));
  }
  // else insert with hint, use it (fallback for vector, deque, list...)
  else {
    container.insert(container.end(), std::forward<T>(value));
  }
}

template <class Container>
concept emplaceable = requires(Container& container) {
  requires (
    requires { container.emplace_back(std::declval<typename Container::value_type>()); } ||
    requires { container.emplace(std::declval<typename Container::value_type>()); } ||
    requires { container.emplace(container.end(), std::declval<typename Container::value_type>()); }
  );
};

template <class Container, class... Args>
void emplace_to(Container& container, Args&&... args) { // NOLINT(*naming*)
  // if has emplace_back, use it (vector, deque, list...)
  if constexpr (requires { container.emplace_back(std::forward<Args>(args)...); }) {
    container.emplace_back(std::forward<Args>(args)...);
  }
  // else if has emplace, use it (set, map, unordered_set, unordered_map...)
  else if constexpr (requires { container.emplace(std::forward<Args>(args)...); }) {
    container.emplace(std::forward<Args>(args)...);
  }
  // else emplace with hint, use it (fallback for vector, deque, list...)
  else {
    container.emplace(container.end(), std::forward<Args>(args)...);
  }
}

} // namespace robocin::builder_internal

#endif // ROBOCIN_MACROS_INTERNAL_BUILDER_INTERNAL_H

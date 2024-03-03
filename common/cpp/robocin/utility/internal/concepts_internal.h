#ifndef ROBOCIN_UTILITY_INTERNAL_CONCEPTS_INTERNAL_H
#define ROBOCIN_UTILITY_INTERNAL_CONCEPTS_INTERNAL_H

#include <concepts>

namespace robocin::concepts_internal {

template <class T>
concept optionalish = requires(T t) {
  typename T::value_type;
  { t.has_value() } -> std::same_as<bool>;
  { t.value() } -> std::same_as<typename T::value_type&>;
};

} // namespace robocin::concepts_internal

#endif // ROBOCIN_UTILITY_INTERNAL_CONCEPTS_INTERNAL_H

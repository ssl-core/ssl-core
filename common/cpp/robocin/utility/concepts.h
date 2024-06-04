#ifndef ROBOCIN_UTILITY_CONCEPTS_H
#define ROBOCIN_UTILITY_CONCEPTS_H

#include <concepts>
#include <string>
#include <type_traits>

namespace robocin {

template <class T>
concept protobufish = requires(std::remove_cvref_t<T> proto) {
  { proto.DebugString() } -> std::convertible_to<std::string>;
  { proto.ShortDebugString() } -> std::convertible_to<std::string>;
};

template <class T>
concept arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;

} // namespace robocin

#endif // ROBOCIN_UTILITY_CONCEPTS_H

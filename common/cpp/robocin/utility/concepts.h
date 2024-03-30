#ifndef ROBOCIN_UTILITY_CONCEPTS_H
#define ROBOCIN_UTILITY_CONCEPTS_H

#include "robocin/utility/internal/concepts_internal.h"

#include <concepts>
#include <google/protobuf/message_lite.h>
#include <string_view>
#include <type_traits>

namespace robocin {

template <class T>
concept protobufish = std::derived_from<std::remove_cvref_t<T>, ::google::protobuf::MessageLite>;

template <class T>
concept arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;

template <class T>
concept fundamentalish = std::is_fundamental_v<T>;

template <class T>
concept stringish = std::convertible_to<T, std::basic_string_view<char>>
                    || std::convertible_to<T, std::basic_string_view<wchar_t>>
                    || std::convertible_to<T, std::basic_string_view<char8_t>>
                    || std::convertible_to<T, std::basic_string_view<char16_t>>
                    || std::convertible_to<T, std::basic_string_view<char32_t>>;

template <class T>
concept optionalish = concepts_internal::optionalish<std::remove_cvref_t<T>>;

} // namespace robocin

#endif // ROBOCIN_UTILITY_CONCEPTS_H

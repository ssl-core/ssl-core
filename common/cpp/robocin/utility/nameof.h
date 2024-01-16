#ifndef ROBOCIN_UTILITY_NAMEOF_H
#define ROBOCIN_UTILITY_NAMEOF_H

#include <algorithm>
#include <array>
#include <string_view>

namespace robocin {

template <class T>
constexpr const char* nameof() noexcept {
  static constexpr std::string_view prefix =
#if defined(__clang__)
      "const char *robocin::nameof() [T = ";
#elif defined(__GNUC__)
      "constexpr const char* robocin::nameof() [with T = ";
#else
#message "robocin::nameof() is not implemented for this compiler."
#endif
  static constexpr std::string_view pretty_function = __PRETTY_FUNCTION__;
  static constexpr std::string_view suffix = "]";

  static_assert(pretty_function.starts_with(prefix) and pretty_function.ends_with(suffix));

  static constexpr std::size_t n = pretty_function.size() - prefix.size() - suffix.size() + 1;
  using result_array_t = std::array<char, n>;

  static constexpr result_array_t result_array = []() {
    result_array_t result{};
    std::copy_n(pretty_function.data() + prefix.size(), n - 1, result.data());
    return result;
  }();

  return result_array.data();
}

} // namespace robocin

#endif // ROBOCIN_UTILITY_NAMEOF_H

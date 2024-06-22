#ifndef ROBOCIN_OUTPUT_INTERNAL_LOG_INTERNAL_H
#define ROBOCIN_OUTPUT_INTERNAL_LOG_INTERNAL_H

#include "robocin/version/version.h" // IWYU pragma: keep

#include <format>
#include <source_location>
#include <string_view>

#if defined(__cpp_lib_print) and __cpp_lib_print >= 202211L
#include <print>
#endif

namespace robocin::log_internal {

#if defined(__cpp_lib_print) and __cpp_lib_print >= 202211L

using std::print;
using std::println;

#else

template <class... Args>
void print(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args) {
  std::fputs(std::format(fmt, std::forward<Args>(args)...).c_str(), stream);
}

template <class... Args>
void print(std::format_string<Args...> fmt, Args&&... args) {
  std::fputs(std::format(fmt, std::forward<Args>(args)...).c_str(), stdout);
}

template <class... Args>
void println(std::FILE* stream, std::format_string<Args...> fmt, Args&&... args) {
  print(stream, fmt, std::forward<Args>(args)...);
  std::fputc('\n', stream);
}

template <class... Args>
void println(std::format_string<Args...> fmt, Args&&... args) {
  print(stdout, fmt, std::forward<Args>(args)...);
  std::fputc('\n', stdout);
}

#endif

template <class Char, class... Args>
class basic_format_string_with_location { // NOLINT(*naming*)
 public:
  template <class Str>
    requires std::convertible_to<const Str&, std::basic_format_string<Char, Args...>>
  consteval basic_format_string_with_location(const Str& str, // NOLINT(*explicit*)
                                              std::source_location location
                                              = std::source_location::current()) :
      str_(str),
      location_(location) {}

  basic_format_string_with_location(std::basic_string_view<Char> str, // NOLINT(*explicit*)
                                    std::source_location location
                                    = std::source_location::current()) :
      str_(str),
      location_(location) {}

  [[nodiscard]] constexpr std::basic_format_string<Char, Args...> get() const { // NOLINT(*naming*)
    return str_;
  }
  [[nodiscard]] constexpr std::source_location location() const { return location_; }

 private:
  std::basic_format_string<Char, Args...> str_;
  std::source_location location_;
};

template <class... Args>
using format_string_with_location // NOLINT(*naming*)
    = basic_format_string_with_location<char, std::type_identity_t<Args>...>;

} // namespace robocin::log_internal

#endif // ROBOCIN_OUTPUT_INTERNAL_LOG_INTERNAL_H

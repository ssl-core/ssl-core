#ifndef EXAMPLE_PRINT_H
#define EXAMPLE_PRINT_H

#include <version>

#if defined(__cpp_lib_print) and __cpp_lib_print >= 202207L

#include <print>

namespace robocin {

using [[deprecated("use 'std::print' instead.")]] std::print;
using [[deprecated("use 'std::println' instead.")]] std::println;

} // namespace robocin

#warning "Replace all robocin::print[ln] occurrences with std::print[ln] and remove this file."

#else

#include <cstdio>
#include <format>

namespace robocin {

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

} // namespace robocin

#endif

#endif // EXAMPLE_PRINT_H

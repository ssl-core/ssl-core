#ifndef ROBOCIN_OUTPUT_LOG_H
#define ROBOCIN_OUTPUT_LOG_H

#include <cstdlib>
#include <format>
#include <utility>

namespace robocin {
namespace log_internal {

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

} // namespace log_internal

// TODO(#126): add source location and fill files with logs.

template <class... Args>
void ilog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  log_internal::println(stdout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void wlog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  log_internal::println(stdout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void elog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  log_internal::println(stderr, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void flog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  log_internal::println(stderr, fmt, std::forward<Args>(args)...);

  std::abort();
}

} // namespace robocin

#endif // ROBOCIN_OUTPUT_LOG_H

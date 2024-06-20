#ifndef ROBOCIN_OUTPUT_LOG_H
#define ROBOCIN_OUTPUT_LOG_H

#include <cstdlib>
#include <format>
#include <print>

namespace robocin {

// TODO(#126): add source location and fill files with logs.

template <class... Args>
void ilog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  std::print(stdout, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  std::println(stdout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void wlog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  std::print(stdout, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  std::println(stdout, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void elog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  std::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  std::println(stderr, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void flog(std::format_string<Args...> fmt, Args&&... args) {
  //  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  //  std::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());

  std::println(stderr, fmt, std::forward<Args>(args)...);

  std::abort();
}

} // namespace robocin

#endif // ROBOCIN_OUTPUT_LOG_H

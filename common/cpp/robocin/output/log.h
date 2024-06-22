#ifndef ROBOCIN_OUTPUT_LOG_H
#define ROBOCIN_OUTPUT_LOG_H

#include "robocin/output/internal/log_internal.h"

#include <cstdlib>
#include <source_location>

namespace robocin {

template <class... Args>
inline void ilog(log_internal::format_string_with_location<Args...> fmt, Args&&... args) {
  const std::source_location& loc = fmt.location();

  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  log_internal::print(stdout, "[{}:{}:{}] ", file_name, loc.line(), loc.column());
  log_internal::println(stdout, fmt.get(), std::forward<Args>(args)...);
}

template <class... Args>
inline void wlog(log_internal::format_string_with_location<Args...> fmt, Args&&... args) {
  const std::source_location& loc = fmt.location();

  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  log_internal::print(stdout, "[{}:{}:{}] ", file_name, loc.line(), loc.column());
  log_internal::println(stdout, fmt.get(), std::forward<Args>(args)...);
}

template <class... Args>
inline void elog(log_internal::format_string_with_location<Args...> fmt, Args&&... args) {
  const std::source_location& loc = fmt.location();

  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());
  log_internal::println(stderr, fmt.get(), std::forward<Args>(args)...);
}

template <class... Args>
inline void flog(log_internal::format_string_with_location<Args...> fmt, Args&&... args) {
  const std::source_location& loc = fmt.location();

  auto file_name = std::string_view{loc.file_name()}.substr(sizeof(ROBOCIN_REPOSITORY_PATH));
  log_internal::print(stderr, "[{}:{}:{}] ", file_name, loc.line(), loc.column());
  log_internal::println(stderr, fmt.get(), std::forward<Args>(args)...);

  std::abort();
}

} // namespace robocin

#endif // ROBOCIN_OUTPUT_LOG_H

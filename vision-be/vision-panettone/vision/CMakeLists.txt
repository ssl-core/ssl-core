robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS absl::time
       absl::synchronization
       absl::random_random
       absl::strings
)

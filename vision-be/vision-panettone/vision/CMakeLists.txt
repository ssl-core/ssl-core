robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS absl::time
       absl::synchronization
       absl::random_random
       absl::strings
       protocols::third_party
       protocols::ui
       protocols::vision
       common::network
       common::thread_pool
       irepository_factory
       repository_factory_mapping
)

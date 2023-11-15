add_subdirectory(common)

robocin_cpp_executable(
    NAME client
    SRCS client_main.cpp
    DEPS zmq_publish_subscribe
         absl::time
         absl::synchronization
         absl::random_random
         absl::strings
         utility
)

robocin_cpp_executable(
  NAME monolith_main
  SRCS monolith_main.cpp
  DEPS common::network
       common::thread_pool
       protocols::third_party
       protocols::messages
       protocols::vision
)

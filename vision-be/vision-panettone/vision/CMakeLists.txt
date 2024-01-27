add_subdirectory(network)

robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS cppzmq network
)

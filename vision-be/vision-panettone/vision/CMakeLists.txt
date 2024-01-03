add_subdirectory(network)

robocin_cpp_executable(
  NAME vision_main.cpp
  SRCS vision_main.cpp
  DEPS cppzmq network
)

robocin_cpp_executable(
  NAME client_main
  SRCS client_main.cpp
  DEPS cppzmq common::network network
)

robocin_cpp_executable(
  NAME service_main
  SRCS service_main.cpp
  DEPS cppzmq gateway common::network network
)

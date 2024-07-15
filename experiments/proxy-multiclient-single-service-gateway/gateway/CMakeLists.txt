robocin_cpp_executable(
  NAME client_main
  SRCS client_main.cpp
  DEPS cppzmq
)

robocin_cpp_executable(
  NAME gateway_main
  SRCS gateway_main.cpp
  DEPS cppzmq
)

robocin_cpp_executable(
  NAME service_main
  SRCS service_main.cpp
  DEPS cppzmq
)

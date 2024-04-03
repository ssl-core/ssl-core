robocin_cpp_library(
  NAME service_discovery
  HDRS service_discovery.h
  SRCS service_discovery.cpp
  DEPS singleton
)

robocin_cpp_executable(
  NAME gateway_main
  SRCS gateway_main.cpp
  DEPS service_discovery controller protocols::gateway
)

robocin_cpp_executable(
  NAME client_main
  SRCS client_main.cpp
  DEPS protocols::gateway
)

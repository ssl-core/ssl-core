robocin_cpp_library(
  NAME service_discovery
  HDRS service_discovery.h
  SRCS service_discovery.cpp
  DEPS singleton
)

robocin_cpp_grpc_library(
  NAME gateway_grpc_lib
  PROTOS gateway.proto
)

robocin_cpp_executable(
  NAME gateway_main
  SRCS gateway_main.cpp
  DEPS service_discovery controller gateway_grpc_lib
)

robocin_cpp_executable(
  NAME client_main
  SRCS client_main.cpp
  DEPS gateway_grpc_lib
)

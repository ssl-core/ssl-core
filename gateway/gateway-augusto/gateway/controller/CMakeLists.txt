robocin_cpp_library(
  NAME controller
  HDRS icontroller.h grpc_controller.h third_party_sockets_controller.h
  SRCS icontroller.cpp grpc_controller.cpp third_party_sockets_controller.cpp
  DEPS cppzmq common::network singleton network gateway_grpc_lib
)

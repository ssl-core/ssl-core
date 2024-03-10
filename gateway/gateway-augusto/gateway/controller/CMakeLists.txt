robocin_cpp_library(
  NAME controllers 
  HDRS controller.h asynchronous_sockets_controller.h synchronous_sockets_controller.h third_party_sockets_controller.h
  SRCS controller.cpp asynchronous_sockets_controller.cpp synchronous_sockets_controller.cpp third_party_sockets_controller.cpp
  DEPS cppzmq common::network singleton network
)

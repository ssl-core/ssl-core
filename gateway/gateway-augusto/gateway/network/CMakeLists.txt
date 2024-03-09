robocin_cpp_library (
  NAME network
  HDRS zmq_router_dealer_socket.h poller.h
  SRCS zmq_router_dealer_socket.cpp poller.cpp
  DEPS cppzmq
)

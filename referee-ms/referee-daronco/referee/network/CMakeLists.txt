robocin_cpp_library (
  NAME zmq_poller
  HDRS zmq_poller.h
  SRCS zmq_poller.cpp
  DEPS cppzmq common::network
)

robocin_cpp_library (
  NAME network
  HDRS zmq_request_reply_socket.h poller.h
  SRCS zmq_request_reply_socket.cpp poller.cpp
  DEPS cppzmq
)

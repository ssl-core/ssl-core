robocin_cpp_library(
    NAME network
    HDRS socket.h udp_multicast_socket_receiver.h zmq_subscriber_socket.h zmq_publisher_socket.h poller.h
    SRCS socket.cpp udp_multicast_socket_receiver.cpp zmq_subscriber_socket.cpp zmq_publisher_socket.cpp poller.cpp
    DEPS cppzmq
)

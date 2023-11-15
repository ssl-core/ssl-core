robocin_cpp_library(
    NAME concepts
    HDRS concepts.h
    SRCS concepts.cpp
    DEPS protobuf::libprotobuf
)

robocin_cpp_library(
    NAME publish_subscribe
    HDRS publish_subscribe.h
    SRCS publish_subscribe.cpp
    DEPS concepts
)

robocin_cpp_library(
    NAME zmq_publish_subscribe
    HDRS zmq_publish_subscribe.h
    SRCS zmq_publish_subscribe.cpp
    DEPS publish_subscribe cppzmq
)

robocin_cpp_library(
    NAME utility
    HDRS utility.h
    SRCS utility.cpp
    DEPS protobuf::libprotobuf absl::time
)

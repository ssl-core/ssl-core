robocin_cpp_library(
  NAME messaging_receiver
  HDRS payload.h
       payload_mapper.h
       message_receiver.h
       ../internal/messaging_internal.h
  SRCS payload.cpp
       payload_mapper.cpp
       message_receiver.cpp
  DEPS common::network
       common::output
       protocols::protobufs
)

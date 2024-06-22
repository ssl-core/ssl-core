robocin_cpp_library(
  NAME messaging_sender
  HDRS message_sender.h
  SRCS message_sender.cpp
  DEPS common::network
       common::output
       protocols::protobufs
)

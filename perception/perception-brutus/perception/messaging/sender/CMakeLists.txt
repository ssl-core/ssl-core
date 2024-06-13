robocin_cpp_library(
  NAME message_sender
  HDRS message_sender.h
  SRCS message_sender.cpp
  DEPS common::network protocols::protobufs
)

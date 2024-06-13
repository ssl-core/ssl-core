robocin_cpp_library(
  NAME payload
  HDRS payload.h
  SRCS payload.cpp
  DEPS common::network
)

robocin_cpp_library(
  NAME message_receiver
  HDRS message_receiver.h
  SRCS message_receiver.cpp
  DEPS payload
)

robocin_cpp_library(
  NAME message_consumer
  HDRS message_consumer.h
  SRCS message_consumer.cpp
  DEPS network
)

robocin_cpp_library(
  NAME message_producer
  HDRS message_producer.h
  SRCS message_producer.cpp
  DEPS network protocols::perception
)

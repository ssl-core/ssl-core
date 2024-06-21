robocin_cpp_library(
  NAME controller
  HDRS icontroller.h
  SRCS icontroller.cpp
)

robocin_cpp_library(
  NAME producer_controller
  HDRS producer_controller.h
  SRCS producer_controller.cpp
  DEPS common::concurrency
       controller
       message_receiver
)

robocin_cpp_library(
  NAME consumer_controller
  HDRS consumer_controller.h
  SRCS consumer_controller.cpp
  DEPS common::concurrency
       controller
       message_sender 
       detection_processor
)

robocin_cpp_library(
  NAME controller
  HDRS icontroller.h
  SRCS icontroller.cpp
)

robocin_cpp_library(
  NAME vision_packets
  HDRS vision_packets.h
  SRCS vision_packets.cpp
  DEPS protocols::third_party common::network
)

robocin_cpp_library(
  NAME vision_packets_processor
  HDRS vision_packets_processor.h
  SRCS vision_packets_processor.cpp
  DEPS vision_packets vision_filter protocols::perception raw_detection_converter
)

robocin_cpp_library(
  NAME producer_controller
  HDRS producer_controller.h
  SRCS producer_controller.cpp
  DEPS message_consumer common::concurrency controller
)

robocin_cpp_library(
  NAME consumer_controller
  HDRS consumer_controller.h
  SRCS consumer_controller.cpp
  DEPS message_producer common::concurrency controller vision_packets_processor
)

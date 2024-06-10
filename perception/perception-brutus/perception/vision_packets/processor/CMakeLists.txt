robocin_cpp_library(
  NAME vision_packets_processor
  HDRS vision_packets_processor.h
  SRCS vision_packets_processor.cpp
  DEPS vision_packets vision_filter protocols::protobufs raw_detection_converter tracked_frame_converter
)

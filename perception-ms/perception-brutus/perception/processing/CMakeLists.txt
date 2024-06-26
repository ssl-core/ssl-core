robocin_cpp_library(
  NAME processing
  HDRS detection_processor.h
  SRCS detection_processor.cpp
  DEPS raw_detection tracked_detection parameters
)

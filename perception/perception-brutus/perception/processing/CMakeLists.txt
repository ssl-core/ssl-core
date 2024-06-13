robocin_cpp_library(
  NAME detection_processor
  HDRS detection_processor.h
  SRCS detection_processor.cpp
  DEPS vision_filter 
       raw_detection_mapper
       tracked_detection_mapper
)

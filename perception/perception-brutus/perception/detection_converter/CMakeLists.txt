robocin_cpp_library(
  NAME raw_detection
  HDRS raw_detection.h
  SRCS raw_detection.cpp
  DEPS raw_ball raw_robot
)

robocin_cpp_library(
  NAME raw_detection_converter
  HDRS raw_detection_converter.h
  SRCS raw_detection_converter.cpp
  DEPS protocols::third_party raw_detection
)

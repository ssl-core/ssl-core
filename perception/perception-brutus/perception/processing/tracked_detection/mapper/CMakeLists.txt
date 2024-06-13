robocin_cpp_library(
  NAME tracked_detection_mapper
  HDRS tracked_detection_mapper.h
  SRCS tracked_detection_mapper.cpp
  DEPS protocols::protobufs GTest::gtest
)

robocin_cpp_test(
  NAME tracked_detection_mapper_test
  SRCS tracked_detection_mapper_test.cpp
  DEPS tracked_detection_mapper
)

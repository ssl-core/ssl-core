robocin_cpp_library(
  NAME tracked_frame_converter
  HDRS tracked_frame_converter.h
  SRCS tracked_frame_converter.cpp
  DEPS protocols::protobufs GTest::gtest
)

robocin_cpp_test(
  NAME tracked_frame_converter_test
  SRCS tracked_frame_converter_test.cpp
  DEPS tracked_frame_converter
)

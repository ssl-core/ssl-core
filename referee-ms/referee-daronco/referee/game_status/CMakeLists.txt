robocin_cpp_library(
  NAME game_status_mapper
  HDRS game_status_mapper.h
  SRCS game_status_mapper.cpp
  DEPS common::object_ptr protocols::protobufs
)

robocin_cpp_test(
  NAME game_status_mapper_test
  SRCS game_status_mapper_test.cpp
  DEPS game_status_mapper
)

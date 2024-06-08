robocin_cpp_library(
  NAME game_event_mapper
  HDRS game_event_mapper.h
  SRCS game_event_mapper.cpp
  DEPS common::object_ptr
)

robocin_cpp_test(
  NAME game_event_mapper_test
  SRCS game_event_mapper_test.cpp
  DEPS game_event_mapper
)

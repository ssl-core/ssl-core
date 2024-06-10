robocin_cpp_library(
  NAME game_events_mapper
  HDRS game_events_mapper.h
  SRCS game_events_mapper.cpp
  DEPS common::object_ptr protocols::protobufs absl::flat_hash_map detection_util
)

robocin_cpp_test(
  NAME game_events_mapper_test
  SRCS game_events_mapper_test.cpp
  DEPS game_events_mapper
)

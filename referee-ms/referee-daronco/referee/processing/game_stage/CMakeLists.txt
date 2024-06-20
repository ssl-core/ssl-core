robocin_cpp_library(
  NAME game_stage_mapper
  HDRS game_stage_mapper.h
  SRCS game_stage_mapper.cpp
  DEPS protocols::protobufs
)

robocin_cpp_test(
  NAME game_stage_mapper_test
  SRCS game_stage_mapper_test.cpp
  DEPS game_stage_mapper
)

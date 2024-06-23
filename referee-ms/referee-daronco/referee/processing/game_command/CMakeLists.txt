robocin_cpp_library(
  NAME game_command_mapper
  HDRS game_command_mapper.h
  SRCS game_command_mapper.cpp
  DEPS common::memory common::output
)

robocin_cpp_test(
  NAME game_command_mapper_test
  SRCS game_command_mapper_test.cpp
  DEPS game_command_mapper
)

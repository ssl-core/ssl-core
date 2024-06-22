robocin_cpp_library(
  NAME game_status_mapper
  HDRS game_status_mapper.h
  SRCS game_status_mapper.cpp
  DEPS common::memory protocols::protobufs game_command_mapper game_events_mapper game_stage_mapper team_status_mapper
)

robocin_cpp_test(
  NAME game_status_mapper_test
  SRCS game_status_mapper_test.cpp
  DEPS game_status_mapper
)

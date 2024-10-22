robocin_cpp_library(
  NAME robot_command_mapper
  HDRS robot_command_mapper.h
  SRCS robot_command_mapper.cpp
  DEPS common::memory
       protocols::protobufs
)

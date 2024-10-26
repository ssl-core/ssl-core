robocin_cpp_library(
  NAME navigation_entities
  HDRS robot_move.h
  SRCS robot_move.cpp
  DEPS common::geometry 
       protocols::protobufs
)

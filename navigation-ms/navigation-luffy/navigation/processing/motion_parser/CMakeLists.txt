robocin_cpp_library(
  NAME motion_parser
  HDRS motion_parser.h
  SRCS motion_parser.cpp
  DEPS protocols::protobufs
       common::utility
       common::geometry
       navigation_utils
       navigation_entities
       parameters
       
)
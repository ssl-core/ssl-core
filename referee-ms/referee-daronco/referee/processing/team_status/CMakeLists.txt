robocin_cpp_library(
  NAME team_status_mapper
  HDRS team_status_mapper.h
  SRCS team_status_mapper.cpp
  DEPS common::object_ptr protocols::protobufs
)

robocin_cpp_test(
  NAME team_status_mapper_test
  SRCS team_status_mapper_test.cpp
  DEPS team_status_mapper
)

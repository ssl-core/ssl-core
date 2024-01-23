add_subdirectory(color)

robocin_cpp_library(
  NAME robot
  SRCS robot.cpp
  DEPS robot.h
)

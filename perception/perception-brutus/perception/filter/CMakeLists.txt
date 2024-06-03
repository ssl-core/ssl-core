robocin_cpp_library(
  NAME ball_filter
  HDRS ball_filter.h
  SRCS ball_filter.cpp
  DEPS raw_ball
)

robocin_cpp_library(
  NAME robot_filter
  HDRS robot_filter.h
  SRCS robot_filter.cpp
  DEPS raw_robot
)

robocin_cpp_library(
  NAME camera_filter
  HDRS camera_filter.h
  SRCS camera_filter.cpp
  DEPS ball_filter
       robot_filter
       raw_detection
       protocols::perception
)

robocin_cpp_library(
  NAME vision_filter
  HDRS vision_filter.h
  SRCS vision_filter.cpp
  DEPS camera_filter
)

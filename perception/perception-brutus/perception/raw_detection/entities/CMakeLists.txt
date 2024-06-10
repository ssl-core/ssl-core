robocin_cpp_library(
  NAME point
  HDRS point.h
  SRCS point.cpp
)

robocin_cpp_library(
  NAME raw_ball
  HDRS raw_ball.h
  SRCS raw_ball.cpp
  DEPS point 
)

robocin_cpp_library(
  NAME raw_robot
  HDRS raw_robot.h robot_id.h
  SRCS raw_robot.cpp robot_id.cpp
  DEPS point
)

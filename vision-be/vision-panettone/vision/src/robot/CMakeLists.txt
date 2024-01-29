add_subdirectory(color)

robocin_cpp_library(
  NAME robot
  SRCS robot.cpp
  HDRS robot.h
  # TODO(cmake): `robot_color` should be a dependency of `robot` library or add it as a subdirectory is enough?
  DEPS robot_color point_2df point_3df source
)

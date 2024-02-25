robocin_cpp_library(
  NAME point2d
  HDRS point2d.h
  SRCS point2d.cpp
)

robocin_cpp_test(
  NAME point2d_test
  SRCS point2d_test.cpp
  DEPS point2d
)

robocin_cpp_library(
  NAME point3d
  HDRS point3d.h
  SRCS point3d.cpp
)

robocin_cpp_test(
  NAME point3d_test
  SRCS point3d_test.cpp
  DEPS point3d
)

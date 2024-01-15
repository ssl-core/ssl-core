robocin_cpp_library(
  NAME parameters
  HDRS parameters.h
  SRCS parameters.cpp
  DEPS point2d point3d absl::btree singleton
)

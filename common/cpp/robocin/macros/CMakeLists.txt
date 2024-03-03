robocin_cpp_library(
  NAME builder
  HDRS builder.h internal/builder_internal.h
  SRCS builder.cpp
  DEPS concepts span
)

robocin_cpp_test(
  NAME builder_test
  SRCS builder_test.cpp
  DEPS builder
)

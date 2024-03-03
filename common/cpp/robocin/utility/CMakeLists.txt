robocin_cpp_library(
  NAME concepts
  HDRS concepts.h internal/concepts_internal.h
  SRCS concepts.cpp
  DEPS protobuf::libprotobuf
)

robocin_cpp_test(
  NAME concepts_test
  SRCS concepts_test.cpp
  DEPS concepts
)

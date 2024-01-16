robocin_cpp_library(
  NAME concepts
  HDRS concepts.h
  SRCS concepts.cpp
  DEPS protobuf::libprotobuf
)

robocin_cpp_test(
  NAME concepts_test
  SRCS concepts_test.cpp
  DEPS concepts
)

robocin_cpp_library(
  NAME singleton
  HDRS singleton.h
  SRCS singleton.cpp
)

robocin_cpp_library(
  NAME nameof
  HDRS nameof.h
  SRCS nameof.cpp
)

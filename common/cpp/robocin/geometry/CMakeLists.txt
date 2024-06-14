robocin_cpp_library(
  NAME angle
  HDRS angle.h internal/angle_internal.h ../utility/internal/fuzzy_compare_internal.h
  SRCS angle.cpp
  DEPS angular fuzzy_compare
)

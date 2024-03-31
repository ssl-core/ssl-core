robocin_cpp_library(
  NAME angle
  HDRS angle.h internal/angle_internal.h ../utility/internal/fuzzy_compare_internal.h
  SRCS angle.cpp
  DEPS angular fuzzy_compare
)


robocin_cpp_library(
  NAME point2d
  HDRS point2d.h internal/point2d_internal.h ../utility/internal/fuzzy_compare_internal.h
  SRCS point2d.cpp
  DEPS fuzzy_compare
)

robocin_cpp_test(
  NAME point2d_test
  HDRS ../utility/internal/test/epsilon_injector.h
  SRCS point2d_test.cpp
  DEPS point2d
)

robocin_cpp_library(
  NAME point3d
  HDRS point3d.h ../utility/internal/fuzzy_compare_internal.h
  SRCS point3d.cpp
  DEPS point2d fuzzy_compare
)

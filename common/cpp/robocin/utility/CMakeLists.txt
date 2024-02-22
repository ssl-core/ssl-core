if (NOT ROBOCIN_FLOAT_EPSILON AND NOT ROBOCIN_DOUBLE_EPSILON AND NOT ROBOCIN_LONG_DOUBLE_EPSILON)
  message(WARNING "No epsilon value has been set: ROBOCIN_FLOAT_EPSILON, ROBOCIN_DOUBLE_EPSILON, ROBOCIN_LONG_DOUBLE_EPSILON.\n"
          "It is recommended to set epsilon values to avoid numerical instabilities.\n"
          "We suggest to set epsilon values to:\n"
          " -- 1e-2F for float;\n"
          " -- 1e-4 for double;\n"
          " -- 1e-6L for long double.")
endif ()

robocin_cpp_library(
  NAME type_traits
  HDRS type_traits.h
  SRCS type_traits.cpp
)

robocin_cpp_test(
  NAME type_traits_test
  SRCS type_traits_test.cpp
  DEPS type_traits
)

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
  NAME fuzzy_compare
  HDRS fuzzy_compare.h
  SRCS fuzzy_compare.cpp
  CONFIGS epsilon.h.in
  DEPS type_traits concepts
)

robocin_cpp_test(
  NAME fuzzy_compare_test
  HDRS internal/test/epsilon_injector.h
  SRCS fuzzy_compare_test.cpp
  DEPS fuzzy_compare
)

robocin_cpp_library(
  NAME angular
  HDRS angular.h
  SRCS angular.cpp
  DEPS concepts
)

robocin_cpp_test(
  NAME angular_test
  HDRS internal/test/epsilon_injector.h
  SRCS angular_test.cpp
  DEPS angular
)

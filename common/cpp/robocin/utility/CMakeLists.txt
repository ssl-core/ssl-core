if (NOT ROBOCIN_NON_DEFAULT_EPSILON)
  if (NOT ROBOCIN_FLOAT_EPSILON)
    set(ROBOCIN_FLOAT_EPSILON "1e-2F")
  endif ()

  if (NOT ROBOCIN_DOUBLE_EPSILON)
    set(ROBOCIN_DOUBLE_EPSILON "1e-4")
  endif ()

  if (NOT ROBOCIN_LONG_DOUBLE_EPSILON)
    set(ROBOCIN_LONG_DOUBLE_EPSILON "1e-6L")
  endif ()
endif ()

if (NOT ROBOCIN_FLOAT_EPSILON AND NOT ROBOCIN_DOUBLE_EPSILON AND NOT ROBOCIN_LONG_DOUBLE_EPSILON)
  message(WARNING "No epsilon value has been set: ROBOCIN_FLOAT_EPSILON, ROBOCIN_DOUBLE_EPSILON, ROBOCIN_LONG_DOUBLE_EPSILON.\n"
          "It is recommended to set epsilon values to avoid numerical instabilities.\n"
          "We suggest to set epsilon values to:\n"
          " -- 1e-2F for float;\n"
          " -- 1e-4 for double;\n"
          " -- 1e-6L for long double.")
endif ()

robocin_cpp_library(
  NAME utility
  HDRS type_traits.h
       concepts.h
       fuzzy_compare.h
       singleton.h
       iproto_convertible.h
  SRCS type_traits.cpp
       concepts.cpp
       fuzzy_compare.cpp
       singleton.cpp
       iproto_convertible.cpp
  CONFIGS epsilon.h.in
)

robocin_cpp_test(
  NAME type_traits_test
  SRCS type_traits_test.cpp
  DEPS utility
)

robocin_cpp_test(
  NAME concepts_test
  SRCS concepts_test.cpp
  DEPS utility protobuf::libprotobuf
)

robocin_cpp_test(
  NAME fuzzy_compare_test
  HDRS internal/test/epsilon_injector.h
  SRCS fuzzy_compare_test.cpp
  DEPS utility
)

robocin_cpp_library(
  NAME angular
  HDRS angular.h
  SRCS angular.cpp
  DEPS utility
)

robocin_cpp_test(
  NAME angular_test
  HDRS internal/test/epsilon_injector.h
  SRCS angular_test.cpp
  DEPS angular
)

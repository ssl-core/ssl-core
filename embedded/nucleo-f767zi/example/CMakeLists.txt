robocin_cpp_library(
    NAME print
    HDRS print.h
    SRCS print.cpp
)

robocin_cpp_nanopb_library(
    NAME example_nanopb
    PROTOS example.proto
    DEPS protocols::common_nanopb
)

robocin_cpp_executable(
    NAME example_main
    SRCS example_main.cpp
    DEPS mbed-os print example_nanopb common::fuzzy_compare
)

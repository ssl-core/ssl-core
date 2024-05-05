robocin_cpp_library(
    NAME print
    HDRS print.h
    SRCS print.cpp
)

robocin_cpp_executable(
    NAME example_main
    SRCS example_main.cpp
    DEPS mbed-os print
)

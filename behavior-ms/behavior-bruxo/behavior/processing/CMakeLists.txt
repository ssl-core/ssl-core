robocin_cpp_library(
    NAME processing
    HDRS behavior_processor.h
    SRCS behavior_processor.cpp
    DEPS common::output
         messages
)

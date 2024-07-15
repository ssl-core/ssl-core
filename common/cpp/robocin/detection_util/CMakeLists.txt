robocin_cpp_library(
  NAME detection_util
  HDRS clock.h
       duration.h
       timestamp.h
       elapsed_timer.h
  SRCS clock.cpp
       duration.cpp
       timestamp.cpp
       elapsed_timer.cpp
  DEPS utility
)

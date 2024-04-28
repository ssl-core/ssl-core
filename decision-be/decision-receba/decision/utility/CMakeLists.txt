robocin_cpp_library(
  NAME utility
  HDRS behavior_tree.h
  SRCS behavior_tree.cpp
  MODS utility.cppm decision_behavior_tree.cppm
)

robocin_cpp_library(
  NAME observer_ptr
  HDRS observer_ptr.h
  SRCS observer_ptr.cpp
)

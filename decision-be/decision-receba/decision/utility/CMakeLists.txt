robocin_cpp_library(
  NAME behavior_tree
  HDRS behavior_tree.h
  SRCS behavior_tree.cpp
)

robocin_cpp_library(
  NAME utility
  MODS utility.cppm
  DEPS models
)

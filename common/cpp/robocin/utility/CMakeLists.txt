robocin_cpp_library(
  NAME utility
  MODS utility.ixx
       concepts.ixx
  DEPS protobuf::libprotobuf
)

robocin_cpp_executable(
  NAME local_target
  SRCS local_target_main.cpp
  DEPS utility
)

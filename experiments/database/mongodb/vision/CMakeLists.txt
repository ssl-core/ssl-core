robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS irepository_factory
       repository_factory_mapping
       command_invoker
)

robocin_cpp_library(
  NAME test_inputs
  HDRS test_inputs.h
  SRCS test_inputs.cpp
  DEPS protocols::vision
)

robocin_cpp_library(
  NAME commands
  HDRS commands.h
  SRCS commands.cpp
  DEPS iframe_repository test_inputs
)

robocin_cpp_library(
  NAME command_invoker
  HDRS command_invoker.h
  SRCS command_invoker.cpp
  DEPS commands
)


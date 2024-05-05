robocin_cpp_library(
  NAME generators
  MODS generators.cppm
       igenerator.cppm
  DEPS observer_ptr
       world
       common::builder
)

robocin_cpp_library(
  NAME generators
  MODS generators.cppm
       igenerator.cppm
       potential_pass_targets_generator.cppm
  DEPS observer_ptr
       world
       common::builder
)

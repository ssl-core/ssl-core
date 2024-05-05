robocin_cpp_library(
  NAME generators
  MODS generators.cppm
       igenerator.cppm
       field_grid_map_generator.cppm
  DEPS observer_ptr
       world
       common::builder
)

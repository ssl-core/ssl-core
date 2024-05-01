robocin_cpp_library(
  NAME world
  MODS world.cppm
       entity.cppm
       robot.cppm
  DEPS common::builder
       common::point2d
       common::point3d
)

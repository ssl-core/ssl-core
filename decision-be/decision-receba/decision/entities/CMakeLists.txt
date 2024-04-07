robocin_cpp_library(
  NAME entities
  MODS entities.cppm world.cppm robot.cppm
  DEPS common::builder common::point2d common::point3d
)

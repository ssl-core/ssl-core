robocin_cpp_library(
  NAME models
  MODS models.cppm world.cppm robot.cppm
  DEPS common::builder common::point2d common::point3d
)

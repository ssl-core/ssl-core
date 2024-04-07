robocin_cpp_library(
  NAME mappers
  MODS mappers.cppm robot_pb_to_robot_mapper.cppm
  DEPS entities protocols::vision common::point2d
)

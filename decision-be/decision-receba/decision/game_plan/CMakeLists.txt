robocin_cpp_library(
  NAME game_plan
  MODS game_plan.cppm
       entity.cppm
       icoach.cppm
       coach.cppm
  DEPS observer_ptr
       evaluators
       world
       common::builder
)

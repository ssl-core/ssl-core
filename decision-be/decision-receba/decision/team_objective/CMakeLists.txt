robocin_cpp_library(
  NAME team_objective
  MODS team_objective.cppm
       build_up_selector.cppm
       counter_attack_selector.cppm
       creation_selector.cppm
       defensive_block_selector.cppm
       enter_defensive_shape_selector.cppm
       establish_possession_selector.cppm
       high_press_selector.cppm
       possession_selector.cppm
       protect_goal_selector.cppm
  DEPS utility entities
)

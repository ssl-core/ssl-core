robocin_cpp_library(
  NAME phase_of_game
  MODS phase_of_game.cppm
       in_possession_selector.cppm
       out_of_possession_selector.cppm
       attacking_transition_selector.cppm
       defensive_transition_selector.cppm
       dispute_selector.cppm
       set_pieces_selector.cppm
  DEPS world utility team_objective
)

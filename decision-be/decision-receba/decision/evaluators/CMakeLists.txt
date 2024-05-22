robocin_cpp_library(
  NAME evaluators
  MODS evaluators.cppm
       ievaluator.cppm
       individual_marking_evaluator.cppm
       score_chance_evaluator.cppm
       pass_interception_evaluator.cppm
       potential_pass_targets_generator_evaluator.cppm
       potential_pass_targets_rater_evaluator.cppm
       ball_handler_action_selector_evaluator.cppm
  DEPS observer_ptr
       world
       common::builder
)

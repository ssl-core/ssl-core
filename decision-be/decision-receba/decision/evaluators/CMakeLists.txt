robocin_cpp_library(
  NAME evaluators
  MODS evaluators.cppm
       ievaluator.cppm
       individual_marking_evaluator.cppm
       score_chance_evaluator.cppm
       pass_interception_evaluator.cppm
       potential_pass_targets_evaluator.cppm
  DEPS observer_ptr
       generators
       world
       common::builder
)

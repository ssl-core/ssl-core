robocin_cpp_library(
  NAME evaluators
  MODS evaluators.cppm
       ievaluator.cppm
       individual_marking_evaluator.cppm
       score_chance_evaluator.cppm
       pass_interception_evaluator.cppm
       potential_pass_targets_evaluator.cppm
       potential_pass_targets_evaluator/igenerator.cppm
       potential_pass_targets_evaluator/potential_pass_targets_generator.cppm
  DEPS observer_ptr
       world
       common::builder
)

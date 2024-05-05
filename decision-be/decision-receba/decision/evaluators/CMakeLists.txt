robocin_cpp_library(
  NAME evaluators
  MODS evaluators.cppm
       ievaluator.cppm
       individual_marking_evaluator.cppm
       score_chance_evaluator.cppm
       pass_interception_evaluator.cppm
  DEPS observer_ptr
       world
       common::builder
)

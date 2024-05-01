robocin_cpp_library(
  NAME evaluators
  MODS evaluators.cppm
       ievaluator.cppm
       individual_marking_evaluator.cppm
  DEPS observer_ptr
       world
       common::builder
)

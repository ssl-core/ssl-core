robocin_cpp_executable(
  NAME decision
  SRCS decision_main.cpp
  DEPS utility decision_selector game_plan
)

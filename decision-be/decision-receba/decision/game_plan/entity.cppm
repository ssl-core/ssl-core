module;

#include "robocin/macros/builder.h"

export module decision.game_plan:entity;

import decision.evaluators;

export namespace decision {

class GamePlan final {
  BuildableWith(Builder);

  Getter(Default, IndividualMarking, IndividualMarking);
  Getter(Default, ScoreChanceMap, ScoreChanceMap);
  Getter(Default, PassInterceptionMap, PassInterceptionMap);
};

class GamePlan::Builder final {
  BuilderOf(GamePlan);

  Setter(IndividualMarking);
  Setter(ScoreChanceMap);
  Setter(PassInterceptionMap);
};

} // namespace decision

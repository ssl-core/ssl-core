module;

#include "robocin/macros/builder.h"

export module decision.game_plan:entity;

import decision.evaluators;

export namespace decision {

class GamePlan final {
  BuildableWith(Builder);

  Getter(Default, IndividualMarking, IndividualMarking);
  Getter(Default, PotentialPassTargets, PotentialPassTargets);
  Getter(Default, ScoreChanceMap, ScoreChanceMap);
  Getter(Default, PassInterceptionMap, PassInterceptionMap);
  Getter(Default, RatedPotentialPassTargets, RatedPotentialPassTargets);
};

class GamePlan::Builder final {
  BuilderOf(GamePlan);

  Setter(IndividualMarking);
  Setter(PotentialPassTargets);
  Setter(ScoreChanceMap);
  Setter(PassInterceptionMap);
  Setter(RatedPotentialPassTargets);
};

} // namespace decision

module;

#include "robocin/macros/builder.h"

export module decision.game_plan:entity;

import decision.evaluators;

export namespace decision {

class GamePlan final {
  BuildableWith(Builder);

  Getter(Default, IndividualMarking, IndividualMarking);
};

class GamePlan::Builder final {
  BuilderOf(GamePlan);

  Setter(IndividualMarking);
};

} // namespace decision

module;

#include "robocin/macros/builder.h"

export module decision.evaluators:individual_marking_evaluator;

import decision.world;
import :ievaluator;

export namespace decision {

class IndividualMarking final {
  BuildableWith(Builder);
};

class IndividualMarking::Builder final {
  BuilderOf(IndividualMarking);
};

class IndividualMarkingEvaluator : public IEvaluator {
 public:
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] IndividualMarking getIndividualMarking() const { return {}; }
};

} // namespace decision

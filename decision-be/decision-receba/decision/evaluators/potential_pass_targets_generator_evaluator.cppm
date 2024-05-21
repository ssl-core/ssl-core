module;

#include "robocin/macros/builder.h"

export module decision.evaluators:potential_pass_targets_generator_evaluator;

import decision.world;
import :ievaluator;

export namespace decision {

class PotentialPassTargets final {
  BuildableWith(Builder);
};

class PotentialPassTargets::Builder final {
  BuilderOf(PotentialPassTargets);
};

class PotentialPassTargetsGeneratorEvaluator : public IEvaluator {
 public:
  // Implement something like the field grid positioning from ssl-unification
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] PotentialPassTargets getPotentialPassTargets() const { return {}; }
};

} // namespace decision

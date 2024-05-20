module;

#include "robocin/macros/builder.h"

export module decision.evaluators:potential_pass_targets_evaluator.potential_pass_targets_generator;

import decision.world;
import :potential_pass_targets_evaluator.igenerator;

export namespace decision {

class PotentialPassTargets final {
  BuildableWith(Builder);
};

class PotentialPassTargets::Builder final {
  BuilderOf(PotentialPassTargets);
};

class PotentialPassTargetsGenerator : public IGenerator {
 public:
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] PotentialPassTargets getPotentialPassTargets() const { return {}; }
};

} // namespace decision

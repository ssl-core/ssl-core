module;

#include "robocin/macros/builder.h"

export module decision.evaluators:potential_pass_targets_evaluator;

import decision.world;
import :ievaluator;
import decision.generators;

export namespace decision {

class PotentialPassTargetsMap final {
  BuildableWith(Builder);
};

class PotentialPassTargetsMap::Builder final {
  BuilderOf(PotentialPassTargetsMap);
};

class PotentialPassTargetsEvaluator : public IEvaluator {
 public:
  PotentialPassTargetsEvaluator() {
    potential_pass_targets_generator_ = make_generator<PotentialPassTargetsGenerator>();
  }

 private:
  std::unique_ptr<PotentialPassTargetsGenerator> potential_pass_targets_generator_;
};

} // namespace decision

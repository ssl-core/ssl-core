module;

#include "decision/utility/observer_ptr.h"
#include "robocin/macros/builder.h"

export module decision.evaluators:score_chance_evaluator;

import decision.world;
import :ievaluator;
import :potential_pass_targets_generator_evaluator;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

class ScoreChanceMap final {
  BuildableWith(Builder);
};

class ScoreChanceMap::Builder final {
  BuilderOf(ScoreChanceMap);
};

class ScoreChanceEvaluator : public IEvaluator {
 public:
  explicit ScoreChanceEvaluator(observer_ptr<PotentialPassTargetsGeneratorEvaluator>
                                    potential_pass_targets_generator_evaluator) :
      potential_pass_targets_generator_evaluator_(potential_pass_targets_generator_evaluator) {}

  // Implement a simplified version of the shadow heuristics
  void run(const World& world) override {
    // PotentialPassTargets potential_pass_targets
    //     = potential_pass_targets_generator_evaluator_->getPotentialPassTargets();
  }

  void reset() override {}

  [[nodiscard]] ScoreChanceMap getScoreChanceMap() const {
    return {};
  }

 private:
  observer_ptr<PotentialPassTargetsGeneratorEvaluator> potential_pass_targets_generator_evaluator_;
};

} // namespace decision

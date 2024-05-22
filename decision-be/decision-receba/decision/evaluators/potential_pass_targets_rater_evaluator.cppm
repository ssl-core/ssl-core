module;

#include "decision/utility/observer_ptr.h"
#include "robocin/macros/builder.h"

export module decision.evaluators:potential_pass_targets_rater_evaluator;

import decision.world;
import :ievaluator;
import :pass_interception_evaluator;
import :score_chance_evaluator;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

class RatedPotentialPassTargets final {
  BuildableWith(Builder);
};

class RatedPotentialPassTargets::Builder final {
  BuilderOf(RatedPotentialPassTargets);
};

class PotentialPassTargetsRaterEvaluator : public IEvaluator {
 public:
  explicit PotentialPassTargetsRaterEvaluator(observer_ptr<PassInterceptionEvaluator> pass_interception_evaluator,
                                              observer_ptr<ScoreChanceEvaluator> score_chance_evaluator) :
                                              pass_interception_evaluator_(pass_interception_evaluator),
                                              score_chance_evaluator_(score_chance_evaluator) {}

  void run(const World& world) override {
    // PassInterceptionMap pass_interception_map = pass_interception_evaluator_->getPassInterceptionMap();
    // ScoreChanceMap score_chance_map = score_chance_evaluator_->getScoreChanceMap();
    // Mix both maps to have the final rating for each point
  }

  void reset() override {}

  [[nodiscard]] RatedPotentialPassTargets getRatedPotentialPassTargets() const { return {}; }

 private:
  observer_ptr<PassInterceptionEvaluator> pass_interception_evaluator_;
  observer_ptr<ScoreChanceEvaluator> score_chance_evaluator_;
};

} // namespace decision

module;

#include "decision/utility/observer_ptr.h"
#include "robocin/macros/builder.h"

export module decision.evaluators:pass_interception_evaluator;

import decision.world;
import :ievaluator;
import :potential_pass_targets_generator_evaluator;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

class PassInterceptionMap final {
  BuildableWith(Builder);
};

class PassInterceptionMap::Builder final {
  BuilderOf(PassInterceptionMap);
};

class PassInterceptionEvaluator : public IEvaluator {
 public:
  explicit PassInterceptionEvaluator(observer_ptr<PotentialPassTargetsGeneratorEvaluator>
                                    potential_pass_targets_generator_evaluator) :
      potential_pass_targets_generator_evaluator_(potential_pass_targets_generator_evaluator) {}

  void run(const World& world) override {
    // PotentialPassTargets potential_pass_targets
    //     = potential_pass_targets_generator_evaluator_->getPotentialPassTargets();
  }

  void reset() override {}

  [[nodiscard]] PassInterceptionMap getPassInterceptionMap() const { return {}; }

 private:
  observer_ptr<PotentialPassTargetsGeneratorEvaluator> potential_pass_targets_generator_evaluator_;
};

} // namespace decision

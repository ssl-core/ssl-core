module;

#include "decision/utility/observer_ptr.h"
#include "robocin/macros/builder.h"

export module decision.evaluators:ball_handler_action_selector_evaluator;

import decision.world;
import :ievaluator;
import :potential_pass_targets_rater_evaluator;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

// TODO: Expand BallHandlerAction to Pass, Kick and InstaKick
class BallHandlerAction final {
  BuildableWith(Builder);
};

class BallHandlerAction::Builder final {
  BuilderOf(BallHandlerAction);
};

class BallHandlerActionSelectorEvaluator : public IEvaluator {
 public:
  explicit BallHandlerActionSelectorEvaluator(observer_ptr<PotentialPassTargetsRaterEvaluator>
                                    potential_pass_targets_rater_evaluator) :
      potential_pass_targets_rater_evaluator_(potential_pass_targets_rater_evaluator) {}

  void run(const World& world) override {
    // RatedPotentialPassTargets rated_potential_pass_targets
    //     = potential_pass_targets_rater_evaluator_->getRatedPotentialPassTargets();
    // TODO: create a generic score evaluator; 
    // evaluate the score chance from the ball position; 
    // decide between shooting on goal or passing
  }

  void reset() override {}

  [[nodiscard]] BallHandlerAction getBallHandlerAction() const { return {}; }

 private:
  observer_ptr<PotentialPassTargetsRaterEvaluator> potential_pass_targets_rater_evaluator_;
};

} // namespace decision

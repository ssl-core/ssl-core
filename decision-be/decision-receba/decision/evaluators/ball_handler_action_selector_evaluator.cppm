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
class BallHandlerAction {
  BuildableWith(Builder);
};

class BallHandlerAction::Builder {
  BuilderOf(BallHandlerAction);
};

class Kick : public BallHandlerAction {};
class Pass : public BallHandlerAction {};
class InstaKick : public BallHandlerAction {};

class BallHandlerActionSelectorEvaluator : public IEvaluator {
 public:
  explicit BallHandlerActionSelectorEvaluator(observer_ptr<PotentialPassTargetsRaterEvaluator>
                                    potential_pass_targets_rater_evaluator) :
      potential_pass_targets_rater_evaluator_(potential_pass_targets_rater_evaluator) {}

  void run(const World& world) override {
    // RatedPotentialPassTargets rated_potential_pass_targets
    //     = potential_pass_targets_rater_evaluator_->getRatedPotentialPassTargets();
    // Use the evaluated chance from the ball position; 
    // decide between shooting on goal or passing
  }

  void reset() override {}

  [[nodiscard]] BallHandlerAction getBallHandlerAction() const { 
    // TODO: how to return an object of the child classes avoiding slicing
    // Pass pass = std::make_unique<Pass>();
    // return std::move(pass);
    return {}; 
  }

 private:
  observer_ptr<PotentialPassTargetsRaterEvaluator> potential_pass_targets_rater_evaluator_;
};

} // namespace decision

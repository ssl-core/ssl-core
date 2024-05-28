module;

#include <memory>

export module decision.game_plan:coach;

import decision.evaluators;
import :entity;
import :icoach;

export namespace decision {

class Coach : public ICoach {
 public:
  Coach() {
    // General:
    ball_interception_evaluator_ = make_evaluator<BallInterceptionEvaluator>();

    // Possible robots
    possible_ball_handler_evaluator_ = make_evaluator<PossibleBallHandlerEvaluator>(ball_interception_evaluator_);

    // defensive:
    individual_marking_evaluator_ = make_evaluator<IndividualMarkingEvaluator>();

    // positioning:
    potential_pass_targets_generator_evaluator_ = make_evaluator<PotentialPassTargetsGeneratorEvaluator>();
    score_chance_ = make_evaluator<ScoreChanceEvaluator>(potential_pass_targets_generator_evaluator_);
    pass_interception_ = make_evaluator<PassInterceptionEvaluator>(potential_pass_targets_generator_evaluator_);
    potential_pass_targets_rater_evaluator_ = make_evaluator<PotentialPassTargetsRaterEvaluator>(pass_interception_, 
                                                                                                 score_chance_);
    ball_handler_action_selector_evaluator_ = make_evaluator<BallHandlerActionSelectorEvaluator>(potential_pass_targets_rater_evaluator_);
  }

  [[nodiscard]] GamePlan getGamePlan() const override {
    return GamePlan::Builder{}
        .setBallInterceptionMap(ball_interception_evaluator_->getBallInterceptionMap())
        .setOrderedPossibleBallHandlerList(possible_ball_handler_evaluator_->getOrderedPossibleBallHandlerList())
        .setIndividualMarking(individual_marking_evaluator_->getIndividualMarking())
        .setPotentialPassTargets(potential_pass_targets_generator_evaluator_->getPotentialPassTargets())
        .setScoreChanceMap(score_chance_->getScoreChanceMap())
        .setPassInterceptionMap(pass_interception_->getPassInterceptionMap())
        .setRatedPotentialPassTargets(potential_pass_targets_rater_evaluator_->getRatedPotentialPassTargets())
        .setBallHandlerAction(ball_handler_action_selector_evaluator_->getBallHandlerAction())
        .build();
  }

 private:
  std::unique_ptr<BallInterceptionEvaluator> ball_interception_evaluator_;
  std::unique_ptr<PossibleBallHandlerEvaluator> possible_ball_handler_evaluator_;
  std::unique_ptr<IndividualMarkingEvaluator> individual_marking_evaluator_;
  std::unique_ptr<PotentialPassTargetsGeneratorEvaluator> potential_pass_targets_generator_evaluator_;
  std::unique_ptr<ScoreChanceEvaluator> score_chance_;
  std::unique_ptr<PassInterceptionEvaluator> pass_interception_;
  std::unique_ptr<PotentialPassTargetsRaterEvaluator> potential_pass_targets_rater_evaluator_;
  std::unique_ptr<BallHandlerActionSelectorEvaluator> ball_handler_action_selector_evaluator_;
};

} // namespace decision

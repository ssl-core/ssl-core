module;

#include <memory>

export module decision.game_plan:coach;

import decision.evaluators;
import :entity;
import :icoach;

namespace {

//

}

export namespace decision {

class Coach : public ICoach {
 public:
  Coach() {
    // defensive:
    individual_marking_evaluator_ = make_evaluator<IndividualMarkingEvaluator>();
    // positioning:
    score_chance_ = make_evaluator<ScoreChanceEvaluator>();
    pass_interception_ = make_evaluator<PassInterceptionEvaluator>();
  }

  [[nodiscard]] GamePlan getGamePlan() const override {
    return GamePlan::Builder{}
        .setIndividualMarking(individual_marking_evaluator_->getIndividualMarking())
        .setScoreChanceMap(score_chance_->getScoreChanceMap())
        .setPassInterceptionMap(pass_interception_->getPassInterceptionMap())
        .build();
  }

 private:
  std::unique_ptr<IndividualMarkingEvaluator> individual_marking_evaluator_;
  std::unique_ptr<ScoreChanceEvaluator> score_chance_;
  std::unique_ptr<PassInterceptionEvaluator> pass_interception_;
};

} // namespace decision

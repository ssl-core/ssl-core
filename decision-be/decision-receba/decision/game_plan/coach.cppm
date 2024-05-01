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
    //
  }

  [[nodiscard]] GamePlan getGamePlan() const override {
    return GamePlan::Builder{}
        .setIndividualMarking(individual_marking_evaluator_->getIndividualMarking())
        .build();
  }

 private:
  std::unique_ptr<IndividualMarkingEvaluator> individual_marking_evaluator_;
};

} // namespace decision

module;

#include "decision/utility/observer_ptr.h"
#include "robocin/macros/builder.h"

export module decision.evaluators:possible_ball_handler_evaluator;

import decision.world;
import :ievaluator;
import :ball_interception_evaluator;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

class OrderedPossibleBallHandlerList final {
  // Stores a list of the ally robots sorted in descending order by who would be the best ball handler
  BuildableWith(Builder);
};

class OrderedPossibleBallHandlerList::Builder final {
  BuilderOf(OrderedPossibleBallHandlerList);
};

class PossibleBallHandlerEvaluator : public IEvaluator {
 public:
  explicit PossibleBallHandlerEvaluator(observer_ptr<BallInterceptionEvaluator>
                                    ball_interception_evaluator) :
      ball_interception_evaluator_(ball_interception_evaluator) {}

  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] OrderedPossibleBallHandlerList getOrderedPossibleBallHandlerList() const { return {}; }

 private:
  observer_ptr<BallInterceptionEvaluator> ball_interception_evaluator_;
};

} // namespace decision

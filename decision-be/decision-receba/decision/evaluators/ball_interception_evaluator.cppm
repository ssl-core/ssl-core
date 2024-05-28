module;

#include "robocin/macros/builder.h"

export module decision.evaluators:ball_interception_evaluator;

import decision.world;
import :ievaluator;

export namespace decision {

class BallInterceptionMap final {
  // Stores a map of ally robot and its interception point
  BuildableWith(Builder);
};

class BallInterceptionMap::Builder final {
  BuilderOf(BallInterceptionMap);
};

class BallInterceptionEvaluator : public IEvaluator {
 public:
  explicit BallInterceptionEvaluator() {}

  void run(const World& world) override {
    // Calculate the points where each ally robot may intercept the ball
    // This will be used to select the best ally ball handler candidate
  }

  void reset() override {}

  [[nodiscard]] BallInterceptionMap getBallInterceptionMap() const { return {}; }
};

} // namespace decision

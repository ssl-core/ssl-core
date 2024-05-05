module;

#include "robocin/macros/builder.h"

export module decision.evaluators:score_chance_evaluator;

import decision.world;
import :ievaluator;

export namespace decision {

class ScoreChanceMap final {
  BuildableWith(Builder);
};

class ScoreChanceMap::Builder final {
  BuilderOf(ScoreChanceMap);
};

class ScoreChanceEvaluator : public IEvaluator {
 public:
  // Implement a simplified version of the shadow heuristics
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] ScoreChanceMap getScoreChanceMap() const { return {}; }
};

} // namespace decision

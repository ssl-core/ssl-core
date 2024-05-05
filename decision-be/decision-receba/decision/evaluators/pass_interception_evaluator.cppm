module;

#include "robocin/macros/builder.h"

export module decision.evaluators:pass_interception_evaluator;

import decision.world;
import :ievaluator;

export namespace decision {

class PassInterceptionMap final {
  BuildableWith(Builder);
};

class PassInterceptionMap::Builder final {
  BuilderOf(PassInterceptionMap);
};

class PassInterceptionEvaluator : public IEvaluator {
 public:
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] PassInterceptionMap getPassInterceptionMap() const { return {}; }
};

} // namespace decision

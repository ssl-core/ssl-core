module;

#include "decision/utility/observer_ptr.h"

#include <concepts>
#include <memory>
#include <vector>
#include <iostream>

export module decision.game_plan:icoach;

import decision.world;
import decision.evaluators;
import :entity;

namespace {

using ::robocin::observer_ptr;

} // namespace

export namespace decision {

class ICoach {
 public:
  ICoach() = default;

  ICoach(const ICoach&) = delete;
  ICoach& operator=(const ICoach&) = delete;

  ICoach(ICoach&&) = default;
  ICoach& operator=(ICoach&&) = default;

  virtual ~ICoach() = default;

  [[nodiscard]] GamePlan run(const World& world) const {
    for (auto evaluator : evaluators_) {
      evaluator->run(world);
    }

    return getGamePlan();
  }

  void reset() {
    for (auto evaluator : evaluators_) {
      evaluator->reset();
    }
  }

 protected:
  [[nodiscard]] virtual GamePlan getGamePlan() const = 0;

  template <std::derived_from<IEvaluator> T, class... Args>
  [[nodiscard]] std::unique_ptr<T> make_evaluator(Args&&... args) { // NOLINT(*naming*)
    auto evaluator = std::make_unique<T>(std::forward<Args>(args)...);
    evaluators_.emplace_back(evaluator);
    return std::move(evaluator);
  }

 private:
  std::vector<observer_ptr<IEvaluator>> evaluators_;
};

} // namespace decision

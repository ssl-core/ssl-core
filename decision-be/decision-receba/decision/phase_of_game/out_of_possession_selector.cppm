module;

#include <memory>

export module decision.phase_of_game:out_of_possession_selector;

import decision.entities;
import decision.utility;
import decision.team_objective;

export namespace decision {

class OutOfPossessionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {
    add(std::make_unique<HighPressSelector>());
    add(std::make_unique<DefensiveBlockSelector>());
    add(std::make_unique<ProtectGoalSelector>());
  }
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

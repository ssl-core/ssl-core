module;

#include <memory>

export module decision.phase_of_game:in_possession_selector;

import decision.entities;
import decision.utility;
import decision.team_objective;

export namespace decision {

class InPossessionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final { add(std::make_unique<BuildUpSelector>()); }
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

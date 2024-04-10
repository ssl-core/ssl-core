module;

export module decision.team_objective:protect_goal_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class ProtectGoalSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

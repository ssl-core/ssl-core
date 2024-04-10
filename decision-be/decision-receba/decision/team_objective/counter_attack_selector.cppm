module;

export module decision.team_objective:counter_attack_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class CounterAttackSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

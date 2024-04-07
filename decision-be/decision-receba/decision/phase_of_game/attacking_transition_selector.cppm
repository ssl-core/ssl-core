module;

export module decision.phase_of_game:attacking_transition_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class AttackingTransitionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

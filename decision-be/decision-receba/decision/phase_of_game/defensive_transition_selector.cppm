module;

export module decision.phase_of_game:defensive_transition_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class DefensiveTransitionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

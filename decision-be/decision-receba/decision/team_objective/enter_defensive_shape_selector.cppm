module;

export module decision.team_objective:enter_defensive_shape_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class EnterDefensiveShapeSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

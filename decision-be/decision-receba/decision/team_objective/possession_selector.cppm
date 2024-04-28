module;

export module decision.team_objective:possession_selector;

import decision.world;
import decision.utility;

export namespace decision {

class PossessionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

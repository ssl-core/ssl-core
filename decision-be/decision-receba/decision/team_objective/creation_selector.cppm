module;

export module decision.team_objective:creation_selector;

import decision.world;
import decision.utility;

export namespace decision {

class CreationSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

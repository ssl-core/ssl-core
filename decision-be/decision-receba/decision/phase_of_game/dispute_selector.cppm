module;

#include <memory>

export module decision.phase_of_game:dispute_selector;

import decision.entities;
import decision.utility;
import decision.team_objective;

export namespace decision {

class DisputeSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

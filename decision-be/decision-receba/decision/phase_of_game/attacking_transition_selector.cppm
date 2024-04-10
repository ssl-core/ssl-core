module;

#include <memory>

export module decision.phase_of_game:attacking_transition_selector;

import decision.entities;
import decision.utility;
import decision.team_objective;

export namespace decision {

class AttackingTransitionSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {
    add(std::make_unique<CounterAttackSelector>());
    add(std::make_unique<EstablishPossessionSelector>());
  }
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

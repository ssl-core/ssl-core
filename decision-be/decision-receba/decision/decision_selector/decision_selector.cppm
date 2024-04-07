module;

#include <memory>

export module decision.decision_selector;

import decision.entities;
import decision.utility;
import decision.phase_of_game;

export namespace decision {

class DecisionSelector : public DecisionBehaviorTree::Rootable<DecisionBehaviorTree::SelectorNode> {
 public:
  void build() override {
    add(std::make_unique<InPossessionSelector>());
    add(std::make_unique<OutOfPossessionSelector>());
    add(std::make_unique<AttackingTransitionSelector>());
    add(std::make_unique<DefensiveTransitionSelector>());
    add(std::make_unique<DisputeSelector>());
    add(std::make_unique<SetPiecesSelector>());
  }
};

} // namespace decision

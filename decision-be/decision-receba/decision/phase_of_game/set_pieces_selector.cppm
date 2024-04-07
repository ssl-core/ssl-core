module;

export module decision.phase_of_game:set_pieces_selector;

import decision.entities;
import decision.utility;

export namespace decision {

class SetPiecesSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

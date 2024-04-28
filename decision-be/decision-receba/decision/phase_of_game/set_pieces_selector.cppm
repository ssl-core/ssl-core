module;

#include <memory>

export module decision.phase_of_game:set_pieces_selector;

import decision.world;
import decision.utility;
import decision.team_objective;

export namespace decision {

class SetPiecesSelector : public DecisionBehaviorTree::SelectorNode {
 public:
  void build() final {}
  [[nodiscard]] bool abort(const World& world) const final { return false; }
};

} // namespace decision

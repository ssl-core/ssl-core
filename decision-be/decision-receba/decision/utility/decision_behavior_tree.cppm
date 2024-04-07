module;

#include "decision/utility/behavior_tree.h" // TODO(joseviccruz): move to common.

export module decision.utility:decision_behavior_tree;

export namespace decision {

class World; // forward declaring World entity.

using DecisionBehaviorTree = robocin::BehaviorTree<int(const World&)>;

} // namespace decision

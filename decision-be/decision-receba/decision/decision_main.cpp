#include <format>
#include <functional>
#include <iostream>
#include <memory>

import decision.utility;
import decision.decision_selector;
import decision.game_plan;

using decision::DecisionBehaviorTree;
using decision::DecisionSelector;
using decision::Coach;

int main() {
  DecisionBehaviorTree tree{std::make_unique<DecisionSelector>()};

  auto dfs_debug = [](this auto&& dfs_debug, const DecisionBehaviorTree::DebugNode& node) -> void {
    for (const auto& child : node.edges()) {
      std::cout << std::format("({}: {}) -> ({}: {})",
                               node.type(),
                               node.name(),
                               child->type(),
                               child->name())
                << std::endl;
      dfs_debug(*child);
    }
  };

  auto root = tree.debug();
  dfs_debug(*root);

  Coach coach;
  auto game_plan = coach.run({});
  coach.reset();

  return 0;
}

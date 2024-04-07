#include <format>
#include <functional>
#include <iostream>
#include <memory>

import decision.utility;
import decision.decision_selector;

using decision::DecisionBehaviorTree;
using decision::DecisionSelector;

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
  return 0;
}

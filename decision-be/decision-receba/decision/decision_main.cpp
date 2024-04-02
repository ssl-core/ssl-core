#include <format>
#include <functional>
#include <iostream>
#include <memory>

import decision.models;
import decision.utility;

using decision::World;

using DecisionBehaviorTree = robocin::BehaviorTree<int(const World&)>;

class MyTask : public DecisionBehaviorTree::TaskNode {
 public:
  MyTask() = default;

  void build() override { std::cout << "Building MyTask" << std::endl; }

  [[nodiscard]] bool abort(const World& world) const override {
    return world.getRobots().empty();
  }

  [[nodiscard]] result_type run(const World& world) const override {
    return {Status::Running, world.getRobots().size()};
  }
};

class MySelector : public DecisionBehaviorTree::SelectorNode {
 public:
  MySelector() = default;

  void build() override {
    std::cout << "Building MySelector" << std::endl;
    add(std::make_unique<MyTask>());
  }

  [[nodiscard]] bool abort(const World& world) const override {
    return world.getRobots().empty();
  }
};

void dfs(const DecisionBehaviorTree::DebugNode& node) {
  for (const auto& child : node.edges()) {
    std::cout << std::format("({}: {}) -> ({}: {})",
                             node.type(),
                             node.name(),
                             child->type(),
                             child->name())
              << std::endl;
    dfs(*child);
  }
}

int main() {
  DecisionBehaviorTree tree(std::make_unique<MySelector>());

  auto result = tree(World::Builder{}.build());
  std::cout << static_cast<int>(result.first) << ' ' << result.second << std::endl;

  auto result2 = tree(World::Builder{}.setRobots({{}, {}}).build());
  std::cout << static_cast<int>(result2.first) << ' ' << result2.second << std::endl;

  auto dbg = tree.debug();
  dfs(*dbg);
  return 0;
}

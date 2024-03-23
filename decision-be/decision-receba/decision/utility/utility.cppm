module;

#include <vector>
#include <utility>
#include <memory>
#include <ranges>

export module decision.utility;

import decision.models;

export namespace decision {

template <class T, class D = std::default_delete<T>>
using unique_not_nullptr = std::unique_ptr<T, D>; // NOLINT(*naming*)

class IPlayFactory {

};

class IStrategyTask {
 public:
  IStrategyTask() = default;

  IStrategyTask(const IStrategyTask&) = delete;
  IStrategyTask& operator=(const IStrategyTask&) = delete;

  IStrategyTask(IStrategyTask&&) = default;
  IStrategyTask& operator=(IStrategyTask&&) noexcept = default;

  virtual ~IStrategyTask() = default;

  [[nodiscard]] virtual bool isRunnable(const World& world) const = 0;
  [[nodiscard]] virtual std::unique_ptr<IPlayFactory> run(const World& world) const = 0;
};

class StrategyNode {
 public:
  explicit StrategyNode(unique_not_nullptr<IStrategyTask> phase) { phases_.push_back(std::move(phase)); }

  StrategyNode(const StrategyNode&) = delete;
  StrategyNode& operator=(const StrategyNode&) = delete;

  StrategyNode(StrategyNode&&) = default;
  StrategyNode& operator=(StrategyNode&&) noexcept = default;

  ~StrategyNode() = default;

  void push(unique_not_nullptr<IStrategyTask> phase) { phases_.push_back(std::move(phase)); }

  std::unique_ptr<IPlayFactory> run(const World& world) const {
    for (const auto& phase : std::ranges::reverse_view{phases_}) {
      if (phase->isRunnable(world)) {
        return phase->run(world);
      }
    }
    std::unreachable();
  }

 private:
  std::vector<unique_not_nullptr<IStrategyTask>> phases_;
};

} // namespace decision

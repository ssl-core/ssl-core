import decision.models;
import decision.utility;

#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <utility>
#include <expected>
#include <vector>

using namespace decision;

///////////////////////////////////////////////////////////////////////////////

class IPhaseOfGame : public IStrategyTask {
 public:
  IPhaseOfGame() = default;

  IPhaseOfGame(const IPhaseOfGame&) = delete;
  IPhaseOfGame& operator=(const IPhaseOfGame&) = delete;

  IPhaseOfGame(IPhaseOfGame&&) = default;
  IPhaseOfGame& operator=(IPhaseOfGame&&) noexcept = default;

  virtual ~IPhaseOfGame() = default;

  [[nodiscard]] virtual bool isRunnable(const World& /*world*/) const = 0;
  [[nodiscard]] virtual std::unique_ptr<IPlayFactory> run(const World& /*world*/) const = 0;
};

class IDefaultPhaseOfGame : public IPhaseOfGame {
 public:
  [[nodiscard]] bool isRunnable(const World& /*world*/) const final { return true; }
};

///////////////////////////////////////////////////////////////////////////////

class PhaseOfGame : public IPhaseOfGame {
 public:
  [[nodiscard]] bool isRunnable(const World& /*world*/) const final { return true; }
  [[nodiscard]] std::unique_ptr<IPlayFactory> run(const World& /*world*/) const final {
    return std::make_unique<IPlayFactory>();
  }
};

class DefaultPhaseOfGame : public IDefaultPhaseOfGame {
 public:
  [[nodiscard]] std::unique_ptr<IPlayFactory> run(const World& /*world*/) const final {
    return std::make_unique<IPlayFactory>();
  }
};

///////////////////////////////////////////////////////////////////////////////

using Halt = DefaultPhaseOfGame;

using InPossession = PhaseOfGame;
using OutOfPossession = PhaseOfGame;
using AttackingTransition = PhaseOfGame;
using DefendingTransition = PhaseOfGame;
using SetPieces = PhaseOfGame;

using Decision = StrategyNode;

Decision makeDecision() {
  Decision decision(std::make_unique<Halt>());

  decision.push(std::make_unique<InPossession>());
  decision.push(std::make_unique<OutOfPossession>());
  decision.push(std::make_unique<AttackingTransition>());
  decision.push(std::make_unique<DefendingTransition>());
  decision.push(std::make_unique<SetPieces>());

  return decision;
}

int main() {
  // std::expected<int, std::string> expt;

  World world;

  Decision dcs = makeDecision();
  auto team_objective = dcs.run(world);

  return 0;
}

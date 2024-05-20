module;

#include <memory>
#include <concepts>

export module decision.evaluators:potential_pass_targets_evaluator.igenerator;

import decision.world;

export namespace decision {

class IGenerator {
 public:
  IGenerator() = default;

  IGenerator(const IGenerator&) = delete;
  IGenerator& operator=(const IGenerator&) = delete;

  IGenerator(IGenerator&&) = default;
  IGenerator& operator=(IGenerator&&) = default;

  virtual ~IGenerator() = default;

  virtual void run(const World& world) = 0;
  virtual void reset() = 0;
};

template <std::derived_from<IGenerator> T, class... Args>
[[nodiscard]] std::unique_ptr<T> make_generator(Args&&... args) { // NOLINT(*naming*)
  auto generator = std::make_unique<T>(std::forward<Args>(args)...);
  return std::move(generator);
}

} // namespace decision

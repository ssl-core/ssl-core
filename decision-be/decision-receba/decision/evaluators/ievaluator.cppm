module;

#include <concepts>
#include <memory>

export module decision.evaluators:ievaluator;

import decision.world;
import decision.generators;

export namespace decision {

class IEvaluator {
 public:
  IEvaluator() = default;

  IEvaluator(const IEvaluator&) = delete;
  IEvaluator& operator=(const IEvaluator&) = delete;

  IEvaluator(IEvaluator&&) = default;
  IEvaluator& operator=(IEvaluator&&) = default;

  virtual ~IEvaluator() = default;

  virtual void run(const World& world) = 0;
  virtual void reset() = 0;

 protected:
  template <std::derived_from<IGenerator> T, class... Args>
  [[nodiscard]] std::unique_ptr<T> make_generator(Args&&... args) { // NOLINT(*naming*)
    auto generator = std::make_unique<T>(std::forward<Args>(args)...);
    return std::move(generator);
  }
};

} // namespace decision

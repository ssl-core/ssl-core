export module decision.generators:igenerator;

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

} // namespace decision

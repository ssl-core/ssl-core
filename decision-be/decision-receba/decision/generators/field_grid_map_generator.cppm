module;

#include "robocin/macros/builder.h"

export module decision.generators:field_grid_generator;

import decision.world;
import :igenerator;

export namespace decision {

class FieldGridMap final {
  BuildableWith(Builder);
};

class FieldGridMap::Builder final {
  BuilderOf(FieldGridMap);
};

class FieldGridMapGenerator : public IGenerator {
 public:
  void run(const World& world) override {}

  void reset() override {}

  [[nodiscard]] FieldGridMap getFieldGridMap() const { return {}; }
};

} // namespace decision

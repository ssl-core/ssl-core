module;

#include "robocin/macros/builder.h"

#include <vector>

export module decision.models:world;

import :robot;

export namespace decision {

class World final {
  BuildableWith(Builder);

  Getter(Robots, std::vector<Robot>);
};

class World::Builder final {
  BuilderOf(World);

  Setter(Robots);
};

} // namespace decision

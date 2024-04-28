module;

#include "robocin/macros/builder.h"

#include <vector>

export module decision.world:world_entity;

import :robot;

export namespace decision {

class World final {
  BuildableWith(Builder);

  Getter(Repeated, Robots, std::vector<Robot>);
};

class World::Builder final {
  BuilderOf(World);

  Setter(Robots);
};

} // namespace decision

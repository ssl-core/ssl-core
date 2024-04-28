module;

#include "robocin/geometry/point2d.h"
#include "robocin/geometry/point3d.h"
#include "robocin/macros/builder.h"

export module decision.world:robot;

using robocin::Point2Df;

export namespace decision {

class Robot final {
  BuildableWith(Builder);

  enum class Color {
    Unspecified,
    Blue,
    Yellow,
  };

  Getter(Default, Id, int);
  Getter(Default, Color, Color);
  Getter(Default, Position, Point2Df);
  Getter(Default, Angle, float);
};

class Robot::Builder final {
  BuilderOf(Robot);

  Setter(Id);
  Setter(Color);
  Setter(Position);
  Setter(Angle);
};

} // namespace decision

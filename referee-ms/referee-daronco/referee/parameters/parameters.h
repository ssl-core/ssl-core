#ifndef REFEREE_PARAMETERS_PARAMETERS_H
#define REFEREE_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace referee {
// NOLINTBEGIN(*comment*)

constinit const auto pRefereePollerTimeoutMs = ::robocin::parameters::View<1>::asInt32(4 /*ms*/);
// TODO(joseviccruz): check pNearTheBallDistance value.
constinit const auto pNearTheBallDistance = ::robocin::parameters::View<2>::asFloat(300.0F);
// TODO(joseviccruz): check pSlowerSpeedOfMovingBall value.
constinit const auto pSlowerSpeedOfMovingBall = ::robocin::parameters::View<3>::asFloat(300.0F);

// NOLINTEND(*comment*)
} // namespace referee

#endif // REFEREE_PARAMETERS_PARAMETERS_H

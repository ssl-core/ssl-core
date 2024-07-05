#ifndef PERCEPTION_PARAMETERS_PARAMETERS_H
#define PERCEPTION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace perception {
// NOLINTBEGIN(*comment*)

constinit const auto pPerceptionPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);
// TODO(matheusvtna): retrieve the vision source from perception parameters.
constinit const auto pUseTrackedDetectionAsSource = ::robocin::parameters::View<2>::asBool(true);
constinit const auto pRobotRadius = ::robocin::parameters::View<3>::asFloat(90);
constinit const auto pRobotHeight = ::robocin::parameters::View<4>::asFloat(140);
constinit const auto pDribblerWidth = ::robocin::parameters::View<5>::asFloat(60);

// NOLINTEND(*comment*)
} // namespace perception

#endif // PERCEPTION_PARAMETERS_PARAMETERS_H

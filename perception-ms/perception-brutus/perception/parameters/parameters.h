#ifndef PERCEPTION_PARAMETERS_PARAMETERS_H
#define PERCEPTION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace perception {
// NOLINTBEGIN(*comment*)

constinit const auto pPerceptionPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);
// TODO(matheusvtna): retrieve the vision source from perception parameters.
constinit const auto pUseTrackedDetectionAsSource = ::robocin::parameters::View<2>::asBool(false);
constinit const auto pRobotRadius = ::robocin::parameters::View<3>::asFloat(90);
constinit const auto pRobotHeight = ::robocin::parameters::View<4>::asFloat(140);
constinit const auto pDribblerWidth = ::robocin::parameters::View<5>::asFloat(60);

// field (default is div a) see: https://robocup-ssl.github.io/ssl-rules/sslrules.html#_dimensions
constinit const auto pFieldLength = ::robocin::parameters::View<6>::asFloat(12'000);
constinit const auto pFieldWidth = ::robocin::parameters::View<7>::asFloat(9'000);
constinit const auto pGoalDepth = ::robocin::parameters::View<8>::asFloat(180);
constinit const auto pGoalWidth = ::robocin::parameters::View<9>::asFloat(1'800);
constinit const auto pPenaltyAreaDepth = ::robocin::parameters::View<10>::asFloat(1'800);
constinit const auto pPenaltyAreaWidth = ::robocin::parameters::View<11>::asFloat(3'600);
constinit const auto pBoundaryWidth = ::robocin::parameters::View<12>::asFloat(300);
constinit const auto pGoalCenterToPenaltyMark = ::robocin::parameters::View<13>::asFloat(8'000);

// NOLINTEND(*comment*)
} // namespace perception

#endif // PERCEPTION_PARAMETERS_PARAMETERS_H

#ifndef PERCEPTION_PARAMETERS_PARAMETERS_H
#define PERCEPTION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace perception {
// NOLINTBEGIN(*comment*)

constinit const auto pPerceptionPollerTimeoutMs = ::robocin::parameters::View<1>::asInt32(4 /*ms*/);
// TODO(matheusvtna): retrieve the vision source from perception parameters.
constinit const auto pUseTrackedDetectionAsSource = ::robocin::parameters::View<2>::asBool(true);

// NOLINTEND(*comment*)
} // namespace perception

#endif // PERCEPTION_PARAMETERS_PARAMETERS_H

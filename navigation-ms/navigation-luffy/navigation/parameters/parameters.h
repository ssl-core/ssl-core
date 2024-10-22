#ifndef NAVIGATION_PARAMETERS_PARAMETERS_H
#define NAVIGATION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace navigation {
// NOLINTBEGIN(*comment*)

constinit const auto pNavigationPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);

// NOLINTEND(*comment*)
} // namespace navigation

#endif // NAVIGATION_PARAMETERS_PARAMETERS_H

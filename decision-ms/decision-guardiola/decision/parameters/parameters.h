#ifndef DECISION_PARAMETERS_PARAMETERS_H
#define DECISION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace decision {
// NOLINTBEGIN(*comment*)

constinit const auto pDecisionPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);

// NOLINTEND(*comment*)
} // namespace decision

#endif // DECISION_PARAMETERS_PARAMETERS_H

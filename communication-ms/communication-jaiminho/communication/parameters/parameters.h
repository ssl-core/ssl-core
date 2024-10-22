#ifndef COMMUNICATION_PARAMETERS_PARAMETERS_H
#define COMMUNICATION_PARAMETERS_PARAMETERS_H

#include <robocin/parameters/parameters.h>

namespace communication {
// NOLINTBEGIN(*comment*)

constinit const auto pCommunicationPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);
} // namespace communication

#endif // COMMUNICATION_PARAMETERS_PARAMETERS_H

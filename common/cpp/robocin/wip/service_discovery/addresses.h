#ifndef ROBOCIN_WIP_SERVICE_DISCOVERY_ADDRESSES_H
#define ROBOCIN_WIP_SERVICE_DISCOVERY_ADDRESSES_H

#include <string_view>

// TODO(matheusvtna, aalmds): dependency injection for service discovery.

namespace robocin::service_discovery {

inline constexpr std::string_view kGatewayAddress = "ipc:///tmp/.ssl-core/gateway.ipc";
inline constexpr std::string_view kRawDetectionTopic = "vision-third-party";
inline constexpr std::string_view kTrackedDetectionTopic = "tracked-third-party";
inline constexpr std::string_view kGameControllerRefereeTopic = "referee-third-party";

inline constexpr std::string_view kPerceptionAddress = "ipc:///tmp/.ssl-core/perception.ipc";
inline constexpr std::string_view kPerceptionDetectionTopic = "detection";
inline constexpr std::string_view kPerceptionDetectionWrapperTopic = "wrapper";

inline constexpr std::string_view kRefereeAddress = "ipc:///tmp/.ssl-core/referee.ipc";
inline constexpr std::string_view kRefereeGameStatusTopic = "game-status";

} // namespace robocin::service_discovery

#endif // ROBOCIN_WIP_SERVICE_DISCOVERY_ADDRESSES_H

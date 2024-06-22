#ifndef PERCEPTION_MESSAGING_INTERNAL_MESSAGING_INTERNAL_H
#define PERCEPTION_MESSAGING_INTERNAL_MESSAGING_INTERNAL_H

#include <array>
#include <string_view>

// TODO(matheusvtna, aalmds): dependency injection for service discovery.

namespace perception::messaging_internal {

inline constexpr std::string_view kGatewayAddress = "ipc:///tmp/gateway-async-backend.ipc";
inline constexpr std::string_view kRawDetectionTopic = "vision-third-party";
inline constexpr std::string_view kTrackedDetectionTopic = "tracked-third-party";
inline constexpr std::array kGatewayTopics = {kRawDetectionTopic, kTrackedDetectionTopic};

inline constexpr std::string_view kDetectionAddress = "ipc:///tmp/perception-async.ipc";
inline constexpr std::string_view kDetectionTopic = "detection";
inline constexpr std::string_view kDetectionWrapperTopic = "detection-wrapper";

} // namespace perception::messaging_internal

#endif // PERCEPTION_MESSAGING_INTERNAL_MESSAGING_INTERNAL_H

#include "navigation/messaging/receiver/payload.h"

namespace navigation {
namespace {

namespace rc {

using ::protocols::behavior::unification::Behavior;
using ::protocols::perception::Detection;

} // namespace rc

} // namespace

Payload::Payload(std::vector<rc::Behavior> behaviors, std::vector<rc::Detection> detections) :
    behaviors_{std::move(behaviors)},
    detections_{std::move(detections)} {}

std::span<const rc::Behavior> Payload::getBehaviors() const { return behaviors_; }
std::span<const rc::Detection> Payload::getDetections() const { return detections_; }

bool Payload::empty() const { return behaviors_.empty() and detections_.empty(); }

} // namespace navigation

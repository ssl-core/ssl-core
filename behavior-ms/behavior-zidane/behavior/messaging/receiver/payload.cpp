#include "behavior/messaging/receiver/payload.h"

namespace behavior {
namespace {

namespace rc {

using ::protocols::decision::Decision;
using ::protocols::perception::Detection;

} // namespace rc

} // namespace

Payload::Payload(std::vector<rc::Detection> detections, std::vector<rc::Decision> decisions) :
    detections_{std::move(detections)},
    decisions_{std::move(decisions)} {}

std::span<const rc::Detection> Payload::getDetectionMessages() const { return detections_; }

std::span<const rc::Decision> Payload::getDecisionMessages() const { return decisions_; }

bool Payload::empty() const { return detections_.empty() and decisions_.empty(); }

} // namespace behavior

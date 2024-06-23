#include "referee/messaging/receiver/payload.h"

namespace referee {
namespace {

namespace rc {

using ::protocols::perception::Detection;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

} // namespace

Payload::Payload(std::vector<rc::Detection> detections,
                 std::vector<tp::Referee> game_controller_referees) :
    detections_{std::move(detections)},
    game_controller_referees_{std::move(game_controller_referees)} {}

std::span<const rc::Detection> Payload::getDetections() const { return detections_; }

std::span<const tp::Referee> Payload::getGameControllerReferees() const {
  return game_controller_referees_;
}

bool Payload::empty() const { return detections_.empty() and game_controller_referees_.empty(); }

} // namespace referee

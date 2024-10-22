#include "decision/messaging/receiver/payload.h"

namespace decision {
namespace {

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

} // namespace

Payload::Payload(std::vector<rc::Detection> detections, std::vector<rc::GameStatus> game_status) :
    detections_{std::move(detections)},
    game_status_{std::move(game_status)} {}

std::span<const rc::Detection> Payload::getDetections() const { return detections_; }

std::span<const rc::GameStatus> Payload::getGameStatus() const { return game_status_; }

bool Payload::empty() const { return detections_.empty() and game_status_.empty(); }

} // namespace decision

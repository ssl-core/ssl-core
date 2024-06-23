#include "referee/processing/referee_processor.h"

#include "referee/messaging/receiver/payload.h"

#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <ranges>

namespace referee {

namespace {

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

std::vector<rc::Detection> detectionFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getDetections) | std::views::join
         | std::ranges::to<std::vector>();
}

std::vector<tp::Referee> refereeFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getGameControllerReferees) | std::views::join
         | std::ranges::to<std::vector>();
}

} // namespace

RefereeProcessor::RefereeProcessor(std::unique_ptr<IGameStatusMapper> game_status_mapper) :
    game_status_mapper_{std::move(game_status_mapper)} {}

rc::GameStatus RefereeProcessor::process(std::span<const Payload> payloads) {
  std::vector<rc::Detection> detections = detectionFromPayloads(payloads);
  if (detections.empty()) {
    // a new package must be generated only when a new detection is received.
    return rc::GameStatus::default_instance();
  }

  if (std::vector<tp::Referee> game_controller_referees = refereeFromPayloads(payloads);
      !game_controller_referees.empty()) {
    last_game_controller_referee_ = std::move(game_controller_referees.back());
  }

  return game_status_mapper_->fromDetectionAndReferee(detections.back(),
                                                      last_game_controller_referee_);
}

} // namespace referee

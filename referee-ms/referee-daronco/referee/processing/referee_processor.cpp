#include "referee/processing/referee_processor.h"

#include "referee/messaging/receiver/payload.h"

#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <protocols/third_party/game_controller/referee.pb.h>
#include <ranges>

namespace referee {

namespace parameters = ::robocin::parameters;
namespace detection_util = ::robocin::detection_util;

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

RefereeProcessor::RefereeProcessor(
    std::unique_ptr<parameters::IHandlerEngine> parameters_handler_engine,
    std::unique_ptr<detection_util::IClockEngine> clock_engine,
    std::unique_ptr<IGameStatusMapper> game_status_mapper) :
    parameters_handler_engine_{std::move(parameters_handler_engine)},
    clock_engine_{std::move(clock_engine)},
    game_status_mapper_{std::move(game_status_mapper)} {}

std::optional<rc::GameStatus> RefereeProcessor::process(std::span<const Payload> payloads) {
  // TODO(joseviccruz): update parameters here from Payload.
  // parameters_handler_engine_->update(parameters_values);

  if (std::vector<tp::Referee> game_controller_referees = refereeFromPayloads(payloads);
      !game_controller_referees.empty()) {
    last_game_controller_referee_ = std::move(game_controller_referees.back());
  }

  if (!last_game_controller_referee_) {
    return std::nullopt;
  }

  std::vector<rc::Detection> detections = detectionFromPayloads(payloads);
  if (detections.empty()) {
    // a new package must be generated only when a new detection is received.
    return std::nullopt;
  }

  const rc::Detection last_detection = detections.back();

  clock_engine_->update(last_detection.framerate(),
                        /*timestamp =*/{
                            last_detection.serial_id(),
                            last_detection.created_at().seconds(),
                            last_detection.created_at().nanos(),
                        });

  return game_status_mapper_->fromDetectionAndReferee(last_detection,
                                                      *last_game_controller_referee_);
}

} // namespace referee

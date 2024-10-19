#include "decision/processing/decision_processor.h"

#include "decision/messaging/receiver/payload.h"

#include <protocols/decision/decision.pb.h>
#include <protocols/common/robot_id.pb.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>
#include <ranges>

namespace decision {

namespace parameters = ::robocin::parameters;

namespace {

using ::robocin::ilog;

namespace rc {

using ::protocols::decision::Decision;
using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;
using ::protocols::decision::Behavior;
using ::protocols::common::RobotId;
using ::protocols::decision::TacticalPlan;
using ::protocols::decision::DefensivePlan;
using ::protocols::decision::OffensivePlan;

} // namespace rc

std::vector<rc::Detection> detectionFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getDetections) | std::views::join
         | std::ranges::to<std::vector>();
}

std::vector<rc::GameStatus> gameStatusFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getGameStatus) | std::views::join
         | std::ranges::to<std::vector>();
}

} // namespace

DecisionProcessor::DecisionProcessor(
    std::unique_ptr<parameters::IHandlerEngine> parameters_handler_engine) :
    parameters_handler_engine_{std::move(parameters_handler_engine)} {}

std::optional<rc::Decision> DecisionProcessor::process(std::span<const Payload> payloads) {
  // TODO(joseviccruz): update parameters here from Payload.
  // parameters_handler_engine_->update(parameters_values);

  std::vector<rc::GameStatus> gameStatus = gameStatusFromPayloads(payloads);
  if (!gameStatus.empty()) {
    ilog("Sem game status");
    const rc::GameStatus last_game_status_ = gameStatus.back();
  }

  std::vector<rc::Detection> detections = detectionFromPayloads(payloads);
  if (detections.empty()) {
    ilog("Sem detection");
    // a new package must be generated only when a new detection is received.
    return std::nullopt;
  }

  const rc::Detection last_detection = detections.back();

  // TODO
  rc::Decision decision;
  rc::Behavior* behavior = decision.add_behavior();
  behavior->set_id(199);

  rc::RobotId* robot_id = behavior->mutable_robot_id();
  robot_id->set_color(rc::RobotId::COLOR_BLUE);
  robot_id->set_number(199);

  rc::TacticalPlan* tplan = decision.mutable_plan();
  
  rc::OffensivePlan* ofPlan = tplan->mutable_offensive();
  rc::DefensivePlan* dfPlan = tplan->mutable_defensive();

  return decision;
}

} // namespace decision

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
  rc::Decision decision_output;

  if (std::vector<rc::GameStatus> gameStatus = gameStatusFromPayloads(payloads);
      !gameStatus.empty()) {
    last_game_status_ = gameStatus.back();
  }

  if(!last_game_status_) {
    return std::nullopt;
  }

  std::vector<rc::Detection> detections = detectionFromPayloads(payloads);
  if (detections.empty()) {
    // a new package must be generated only when a new detection is received.
    return std::nullopt;
  }

  const rc::Detection last_detection = detections.back();

  ///////////////////////////////////////////////////////////////////////////////////

  // TODO: Implement the logic to generate the decision based on the last detection and the last game status.
  for(const auto& robot : last_detection.robots()) {
    rc::Behavior* behavior = decision_output.add_behavior();

    behavior->set_id(0);    
    behavior->mutable_robot_id()->CopyFrom(robot.robot_id());
  }

  rc::TacticalPlan* tplan = decision_output.mutable_plan();
  
  rc::OffensivePlan* ofPlan = tplan->mutable_offensive();
  rc::DefensivePlan* dfPlan = tplan->mutable_defensive();

  ///////////////////////////////////////////////////////////////////////////////////

  return decision_output;
}

} // namespace decision

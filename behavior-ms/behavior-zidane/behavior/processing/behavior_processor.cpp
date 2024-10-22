#include "behavior/processing/behavior_processor.h"

#include "behavior/messaging/receiver/payload.h"
#include "behavior/parameters/parameters.h"

#include <protocols/behavior/motion.pb.h>
#include <protocols/behavior/behavior_unification.pb.h>

#include <protocols/decision/decision.pb.h>

#include <protocols/perception/detection.pb.h>

#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>
#include <ranges>

namespace behavior {

namespace parameters = ::robocin::parameters;

namespace {

using ::robocin::ilog;
using ::robocin::object_ptr;

namespace rc {


using ::protocols::common::RobotId;
using ::protocols::perception::Ball;
using ::protocols::perception::Detection;
using ::protocols::decision::Decision;

using ::protocols::behavior::GoToPoint;
using ::protocols::behavior::unification::Motion;
using ::protocols::behavior::unification::Output;
using ::protocols::behavior::unification::Behavior;

} // namespace rc

std::vector<rc::Detection> detectionFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getDetectionMessages) | std::views::join
         | std::ranges::to<std::vector>();
}

std::vector<rc::Decision> decisionfromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getDecisionMessages) | std::views::join
         | std::ranges::to<std::vector>();
}

} // namespace

BehaviorProcessor::BehaviorProcessor(
    std::unique_ptr<parameters::IHandlerEngine> parameters_handler_engine) :
    parameters_handler_engine_{std::move(parameters_handler_engine)} {}

std::optional<rc::Behavior> BehaviorProcessor::process(std::span<const Payload> payloads) {
  rc::Behavior behavior_output;

  if(std::vector<rc::Decision> decision_messages = decisionfromPayloads(payloads);
    !decision_messages.empty()) {
    last_decision_ = decision_messages.back();
  }

  if(!last_decision_) {
    return std::nullopt;
  }

  std::vector<rc::Detection> detection_messages = detectionFromPayloads(payloads);
  if(detection_messages.empty()) {
    // a new package must be generated only when a new detection is received.
    return std::nullopt;
  }
  const rc::Detection last_detection = detection_messages.back();
  

  // TODO: implement the logic to generate the behavior based on the last detection and the last decision
  ///////////////////////////////////////////////////////////////////////////////////
  for(const auto& behavior_ : last_decision_->behavior()) {
    rc::Output output;
    output.mutable_robot_id()->CopyFrom(behavior_.robot_id());
    
    if(behavior_.id() == 0) {
      rc::Ball ball = last_detection.balls(last_detection.balls_size() - 1);

      auto* go_to_point = output.mutable_motion()->mutable_go_to_point();
      go_to_point->mutable_target()->set_x(ball.position().x());
      go_to_point->mutable_target()->set_y(ball.position().y());
      go_to_point->set_target_angle(0.0);
    } else {
      auto* go_to_point = output.mutable_motion()->mutable_go_to_point();
      go_to_point->mutable_target()->set_x(0.0);
      go_to_point->mutable_target()->set_y(0.0);
      go_to_point->set_target_angle(0.0);
    }

    behavior_output.add_output()->CopyFrom(output);
  }

  ///////////////////////////////////////////////////////////////////////////////////

  return behavior_output;
}

} // namespace behavior

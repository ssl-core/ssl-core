#include "behavior/processing/behavior_processor.h"

#include "behavior/messaging/receiver/payload.h"
#include "behavior/parameters/parameters.h"

#include <algorithm>
#include <iterator>
#include <print>
#include <protocols/behavior/behavior_unification.pb.h>
#include <protocols/decision/decision.pb.h>
#include <protocols/perception/detection.pb.h>
#include <ranges>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace behavior {

namespace parameters = ::robocin::parameters;

namespace {

using ::robocin::ilog;
using ::robocin::object_ptr;

namespace rc {

using ::protocols::behavior::unification::Behavior;
using ::protocols::decision::Decision;
using ::protocols::perception::Detection;

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
  std::vector<rc::Decision> decision_messages = decisionfromPayloads(payloads);
  std::vector<rc::Detection> detection_messages = detectionFromPayloads(payloads);

  // todo: implement process
  return rc::Behavior{};
}

} // namespace behavior

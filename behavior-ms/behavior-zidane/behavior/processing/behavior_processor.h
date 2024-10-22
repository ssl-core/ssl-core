#ifndef BEHAVIOR_PROCESSING_BEHAVIOR_PROCESSOR_H
#define BEHAVIOR_PROCESSING_BEHAVIOR_PROCESSOR_H

#include "behavior/messaging/receiver/payload.h"

#include <protocols/behavior/behavior_unification.pb.h>
#include <protocols/decision/decision.pb.h>
#include <protocols/perception/detection.pb.h>
#include <robocin/parameters/parameters.h>

namespace behavior {

class IBehaviorProcessor {
 public:
  IBehaviorProcessor() = default;

  IBehaviorProcessor(const IBehaviorProcessor&) = delete;
  IBehaviorProcessor& operator=(const IBehaviorProcessor&) = delete;
  IBehaviorProcessor(IBehaviorProcessor&&) = default;
  IBehaviorProcessor& operator=(IBehaviorProcessor&&) = default;

  virtual ~IBehaviorProcessor() = default;

  virtual std::optional<::protocols::behavior::unification::Behavior>
  process(std::span<const Payload> payloads) = 0;
};

class BehaviorProcessor : public IBehaviorProcessor {
 public:
  explicit BehaviorProcessor(
      std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine);

  std::optional<::protocols::behavior::unification::Behavior>
  process(std::span<const Payload> payloads) override;

 private:
  std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine_;
  std::optional<::protocols::decision::Decision> last_decision_;
};

} // namespace behavior

#endif // BEHAVIOR_PROCESSING_BEHAVIOR_PROCESSOR_H

#ifndef DECISION_PROCESSING_DECISION_PROCESSOR_H
#define DECISION_PROCESSING_DECISION_PROCESSOR_H

#include "decision/messaging/receiver/payload.h"

#include <protocols/decision/decision.pb.h>
#include <protocols/perception/detection.pb.h>
#include <robocin/parameters/parameters.h>

namespace decision {

class IDecisionProcessor {
 public:
  IDecisionProcessor() = default;

  IDecisionProcessor(const IDecisionProcessor&) = delete;
  IDecisionProcessor& operator=(const IDecisionProcessor&) = delete;
  IDecisionProcessor(IDecisionProcessor&&) = default;
  IDecisionProcessor& operator=(IDecisionProcessor&&) = default;

  virtual ~IDecisionProcessor() = default;

  virtual std::optional<::protocols::decision::Decision> process(std::span<const Payload> payloads)
      = 0;
};

class DecisionProcessor : public IDecisionProcessor {
 public:
  explicit DecisionProcessor(
      std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine);

  std::optional<::protocols::decision::Decision>
  process(std::span<const Payload> payloads) override;

 private:
  std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine_;
};

} // namespace decision

#endif // DECISION_PROCESSING_DDECISION_PROCESSOR_H

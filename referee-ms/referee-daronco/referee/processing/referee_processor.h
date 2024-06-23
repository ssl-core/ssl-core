#ifndef REFEREE_PROCESSING_REFEREE_PROCESSOR_H
#define REFEREE_PROCESSING_REFEREE_PROCESSOR_H

#include "referee/messaging/receiver/payload.h"
#include "referee/processing/game_status/game_status_mapper.h"

#include <protocols/perception/detection.pb.h>
#include <protocols/referee/game_status.pb.h>

namespace referee {

class IRefereeProcessor {
 public:
  IRefereeProcessor() = default;

  IRefereeProcessor(const IRefereeProcessor&) = delete;
  IRefereeProcessor& operator=(const IRefereeProcessor&) = delete;
  IRefereeProcessor(IRefereeProcessor&&) = default;
  IRefereeProcessor& operator=(IRefereeProcessor&&) = default;

  virtual ~IRefereeProcessor() = default;

  virtual ::protocols::referee::GameStatus process(std::span<const Payload> payloads) = 0;
};

class RefereeProcessor : public IRefereeProcessor {
 public:
  explicit RefereeProcessor(std::unique_ptr<IGameStatusMapper> game_status_mapper);

  ::protocols::referee::GameStatus process(std::span<const Payload> payloads) override;

 private:
  ::protocols::third_party::game_controller::Referee last_game_controller_referee_;
  std::unique_ptr<IGameStatusMapper> game_status_mapper_;
};

} // namespace referee

#endif // REFEREE_PROCESSING_DETECTION_PROCESSOR_H

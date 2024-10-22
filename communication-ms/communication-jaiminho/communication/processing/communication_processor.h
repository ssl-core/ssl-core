#ifndef COMMUNICATION_PROCESSING_COMMUNICATION_PROCESSOR_H
#define COMMUNICATION_PROCESSING_COMMUNICATION_PROCESSOR_H

#include "communication/messaging/receiver/payload.h"
#include "communication/processing/robot_command/robot_command_mapper.h"

#include <protocols/communication/robot_info.pb.h>
#include <robocin/parameters/parameters.h>

namespace communication {

class ICommunicationProcessor {
 public:
  ICommunicationProcessor() = default;

  ICommunicationProcessor(const ICommunicationProcessor&) = delete;
  ICommunicationProcessor& operator=(const ICommunicationProcessor&) = delete;
  ICommunicationProcessor(ICommunicationProcessor&&) = default;
  ICommunicationProcessor& operator=(ICommunicationProcessor&&) = default;

  virtual ~ICommunicationProcessor() = default;

  virtual std::optional<::protocols::communication::RobotInfo> process(std::span<const Payload> payloads)
      = 0;
};

class CommunicationProcessor : public ICommunicationProcessor {
 public:
  explicit CommunicationProcessor(
      std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine,
      std::unique_ptr<IRobotCommandMapper> robot_command_mapper);

  std::optional<::protocols::communication::RobotInfo>
  process(std::span<const Payload> payloads) override;

 private:
  std::unique_ptr<::robocin::parameters::IHandlerEngine> parameters_handler_engine_;
  std::unique_ptr<IRobotCommandMapper> robot_command_mapper_;

  std::optional<::protocols::third_party::game_controller::Referee> last_game_controller_referee_;
};

} // namespace communication

#endif // COMMUNICATION_PROCESSING_COMMUNICATION_PROCESSOR_H

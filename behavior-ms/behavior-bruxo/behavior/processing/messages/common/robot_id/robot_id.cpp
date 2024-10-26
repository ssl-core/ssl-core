#include "behavior/processing/messages/common/robot_id/robot_id.h"

namespace behavior {

RobotIdMessage::RobotIdMessage(std::optional<Color> color, uint32_t number) :
    color{std::move(color)},
    number{std::move(number)} {}

} // namespace behavior
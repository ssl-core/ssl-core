#include "behavior/processing/messages/behavior/behavior_message.h"

namespace behavior {

OutputMessage::OutputMessage(RobotIdMessage robot_id,
                             MotionMessage motion,
                             PlanningMessage planning) :
    robot_id{std::move(robot_id)},
    motion{std::move(motion)},
    planning{std::move(planning)} {}

BehaviorMessage::BehaviorMessage(std::vector<OutputMessage> output) :
    output{std::move(output)} {}

}    // namespace behavior
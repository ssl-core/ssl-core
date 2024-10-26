#include "behavior/processing/messages/motion/motion_message.h"
#include "behavior/processing/messages/common/robot_id/robot_id.h"
#include "behavior/processing/messages/planning/planning_message.h"

#include <protocols/behavior/behavior_unification.pb.h>

#include <robocin/output/log.h>
#include <robocin/utility/iproto_convertible.h>

namespace behavior {

class OutputMessage : public robocin::IProtoConvertible<protocols::behavior::unification::Output> {
  public:
    OutputMessage(RobotIdMessage robot_id = RobotIdMessage{}, 
                  MotionMessage motion = MotionMessage{},
                  PlanningMessage planning = PlanningMessage{});

    [[nodiscard]] protocols::behavior::unification::Output toProto() const override { 
        protocols::behavior::unification::Output output;
            output.mutable_robot_id()->CopyFrom(robot_id.toProto());
            output.mutable_motion()->CopyFrom(motion.toProto());
            output.mutable_planning()->CopyFrom(planning.toProto());
        return output;
    };

    void fromProto(protocols::behavior::unification::Output output) override {
        robot_id.fromProto(output.robot_id());
        motion.fromProto(output.motion());
        planning.fromProto(output.planning());
    };

    RobotIdMessage robot_id;
    MotionMessage motion;
    PlanningMessage planning;
};

class BehaviorMessage : public robocin::IProtoConvertible<protocols::behavior::unification::Behavior> {
  public:
    BehaviorMessage(std::vector<OutputMessage> output = std::vector<OutputMessage>{});

    [[nodiscard]] protocols::behavior::unification::Behavior toProto() const override {
        protocols::behavior::unification::Behavior behavior;
        for (const auto& output : output) {
            behavior.add_output()->CopyFrom(output.toProto());
        }
        return behavior;
    };

    void fromProto(protocols::behavior::unification::Behavior behavior) override {
        // Implementação específica para converter de proto para BehaviorMessage
    };

    std::vector<OutputMessage> output;
};

}    // namespace behavior
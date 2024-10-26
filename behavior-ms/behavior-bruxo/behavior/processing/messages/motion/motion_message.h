#ifndef BEHAVIOR_PROCESSING_MESSAGES_MOTION_MOTION_MESSAGE_H
#define BEHAVIOR_PROCESSING_MESSAGES_MOTION_MOTION_MESSAGE_H

#include <robocin/utility/iproto_convertible.h>
#include <robocin/geometry/point2d.h>

#include <protocols/behavior/motion.pb.h>
#include <protocols/behavior/behavior_unification.pb.h>

namespace behavior {

class GoToPointMessage : public robocin::IProtoConvertible<protocols::behavior::GoToPoint> {

    enum MovingProfile {
        // Move safely profile
        SafeInStopSpeed = 0,

        // Specific movement profile
        BalancedInApproachBallSpeed = 1,
        BalancedInCarryBallSpeed = 2,
        BalancedInCarryBallLowSpeed = 3,

        // Goto point direct profile
        DirectApproachBallSpeed = 4,
        DirectSafeKickBallSpeed = 5,
        DirectBalancedKickBallSpeed = 6,
        DirectKickBallwithEnemyClose = 7,
        gotoBallWithEnemy = 8,
        PushBallInPenalty = 9,
        TOTOZINHO = 10,

        // Default movement profile
        BalancedInSlowSpeed = 11,
        BalancedInMedianSpeed = 12,
        BalancedInDefaultSpeed = 13,
        BalancedInHighSpeed = 14,

        // Goalkepper profile
        GoalkeeperInTopSpeed = 15,

        // Penalty push ball
        PenaltyPushBall = 16,
    };

    enum PrecisionToTarget {
        HIGH = 0,
        NORMAL = 1,
    };

  public:
    explicit GoToPointMessage(std::optional<robocin::Point2D<float>> target = std::nullopt,
                              std::optional<double> target_angle = std::nullopt,
                              std::optional<MovingProfile> moving_profile = std::nullopt,
                              std::optional<PrecisionToTarget> precision_to_target = std::nullopt,
                              std::optional<bool> sync_rotate_with_linear_movement = std::nullopt);

    [[nodiscard]] protocols::behavior::GoToPoint toProto() const override {
        protocols::behavior::GoToPoint go_to_point;

        if (target.has_value()) {
            go_to_point.mutable_target()->set_x(target->x);
            go_to_point.mutable_target()->set_y(target->y);
        }
        if (target_angle.has_value()) {
            go_to_point.set_target_angle(target_angle.value());
        }
        if (moving_profile.has_value()) {
            go_to_point.set_moving_profile(static_cast<protocols::behavior::MovingProfile>(moving_profile.value_or(MovingProfile::SafeInStopSpeed)));
        }
        if (precision_to_target.has_value()) {
            go_to_point.set_precision_to_target(static_cast<protocols::behavior::PrecisionToTarget>(precision_to_target.value_or(PrecisionToTarget::NORMAL)));
        }
        if (sync_rotate_with_linear_movement.has_value()) {
            go_to_point.set_sync_rotate_with_linear_movement(sync_rotate_with_linear_movement.value());
        }
    
        return go_to_point;
    };

    void fromProto(protocols::behavior::GoToPoint go_to_point) override {
        target = robocin::Point2D<float>{go_to_point.target().x(), go_to_point.target().y()};
        target_angle = go_to_point.target_angle();
        moving_profile = static_cast<MovingProfile>(go_to_point.moving_profile());
        precision_to_target = static_cast<PrecisionToTarget>(go_to_point.precision_to_target());
        sync_rotate_with_linear_movement = go_to_point.sync_rotate_with_linear_movement();
    };

    std::optional<robocin::Point2D<float>> target;
    std::optional<double> target_angle;
    std::optional<MovingProfile> moving_profile;
    std::optional<PrecisionToTarget> precision_to_target;
    std::optional<bool> sync_rotate_with_linear_movement;
};

class GoToPointWithTrajectoryMessage : public robocin::IProtoConvertible<protocols::behavior::GoToPointWithTrajectory> {
  public:
    GoToPointWithTrajectoryMessage();

    [[nodiscard]] protocols::behavior::GoToPointWithTrajectory toProto() const override {
        protocols::behavior::GoToPointWithTrajectory go_to_point_with_trajectory;
        return go_to_point_with_trajectory;
    };

    void fromProto(protocols::behavior::GoToPointWithTrajectory go_to_point_with_trajectory) override {
        // Implementação específica para converter de proto para GoToPointWithTrajectoryMessage
    };
};

class RotateInPointMessage : public robocin::IProtoConvertible<protocols::behavior::RotateInPoint> {
  public:
    RotateInPointMessage();

    [[nodiscard]] protocols::behavior::RotateInPoint toProto() const override {
        protocols::behavior::RotateInPoint rotate_in_point;
        return rotate_in_point;
    };

    void fromProto(protocols::behavior::RotateInPoint rotate_in_point) override {
        // Implementação específica para converter de proto para RotateInPointMessage
    };
};

class RotateOnSelfMessage : public robocin::IProtoConvertible<protocols::behavior::RotateOnSelf> {
  public:
    RotateOnSelfMessage();

    [[nodiscard]] protocols::behavior::RotateOnSelf toProto() const override {
        protocols::behavior::RotateOnSelf rotate_on_self;
        return rotate_on_self;
    };

    void fromProto(protocols::behavior::RotateOnSelf rotate_on_self) override {
        // Implementação específica para converter de proto para RotateOnSelfMessage
    };
};

class MotionMessage : public robocin::IProtoConvertible<protocols::behavior::unification::Motion> {
  public:
    MotionMessage(std::optional<GoToPointMessage> go_to_point = std::nullopt,
                  std::optional<GoToPointWithTrajectoryMessage> go_to_point_with_trajectory = std::nullopt,
                  std::optional<RotateInPointMessage> rotate_in_point = std::nullopt,
                  std::optional<RotateOnSelfMessage> rotate_on_self = std::nullopt);

    [[nodiscard]] protocols::behavior::unification::Motion toProto() const override {
        protocols::behavior::unification::Motion motion;

        if (go_to_point.has_value()) {
            motion.mutable_go_to_point()->CopyFrom(go_to_point->toProto());
        }
        if (go_to_point_with_trajectory.has_value()) {
            motion.mutable_go_to_point_with_trajectory()->CopyFrom(go_to_point_with_trajectory->toProto());
        }
        if (rotate_in_point.has_value()) {
            motion.mutable_rotate_in_point()->CopyFrom(rotate_in_point->toProto());
        }
        if (rotate_on_self.has_value()) {
            motion.mutable_rotate_on_self()->CopyFrom(rotate_on_self->toProto());
        }
    
        return motion;
    };

    void fromProto(protocols::behavior::unification::Motion motion) override {
        if (motion.has_go_to_point()) {
            go_to_point = GoToPointMessage{};
            go_to_point->fromProto(motion.go_to_point());
        }
        if (motion.has_go_to_point_with_trajectory()) {
            go_to_point_with_trajectory = GoToPointWithTrajectoryMessage{};
            go_to_point_with_trajectory->fromProto(motion.go_to_point_with_trajectory());
        }
        if (motion.has_rotate_in_point()) {
            rotate_in_point = RotateInPointMessage{};
            rotate_in_point->fromProto(motion.rotate_in_point());
        }
        if (motion.has_rotate_on_self()) {
            rotate_on_self = RotateOnSelfMessage{};
            rotate_on_self->fromProto(motion.rotate_on_self());
        }
    };

    std::optional<GoToPointMessage> go_to_point;
    std::optional<GoToPointWithTrajectoryMessage> go_to_point_with_trajectory;
    std::optional<RotateInPointMessage> rotate_in_point;
    std::optional<RotateOnSelfMessage> rotate_on_self;
};

} // namespace behavior

#endif // BEHAVIOR_PROCESSING_MESSAGES_MOTION_MOTION_MESSAGE_H

package mappers

import (
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/playback"
)

func DetectionMapper(input *perception.Detection) *playback.Detection {
	if input == nil {
		return nil
	}

	return &playback.Detection{
		SerialId:          input.SerialId,
		CreatedAt:         input.CreatedAt,
		ExpectedFramerate: input.Framerate,
		Balls:             ballsMapper(input.Balls),
		Robots:            robotsMapper(input.Robots),
	}
}

func ballsMapper(input []*perception.Ball) []*playback.Detection_Ball {
	output := make([]*playback.Detection_Ball, 0)

	for _, ball := range input {
		output = append(output, &playback.Detection_Ball{
			Confidence:      ball.Confidence,
			Position:        ball.Position,
			Velocity:        ball.Velocity,
			KickInformation: ballKickInformationMapper(ball.KickInformation),
		})
	}

	return output
}

func ballKickInformationMapper(input *perception.Ball_KickInformation) *playback.Detection_Ball_KickInformation {
	if input == nil {
		return nil
	}

	return &playback.Detection_Ball_KickInformation{
		RobotId:                input.RobotId,
		StartPosition:          input.StartPosition,
		StartVelocity:          input.StartVelocity,
		StartTimestamp:         input.StartTimestamp,
		PredictedStopPosition:  input.PredictedStopPosition,
		PredictedStopTimestamp: input.PredictedStopTimestamp,
	}
}

func robotsMapper(input []*perception.Robot) []*playback.Detection_Robot {
	output := make([]*playback.Detection_Robot, 0)

	for _, robot := range input {
		output = append(output, &playback.Detection_Robot{
			Confidence:      robot.Confidence,
			RobotId:         robot.RobotId,
			Position:        robot.Position,
			Angle:           robot.Angle,
			Velocity:        robot.Velocity,
			AngularVelocity: robot.AngularVelocity,
			Radius:          robot.Radius,
			Height:          robot.Height,
			DribblerWidth:   robot.DribblerWidth,
			Feedback:        robotFeedbackMapper(robot.Feedback),
		})
	}

	return output
}

func robotFeedbackMapper(input *perception.Robot_Feedback) *playback.Detection_Robot_Feedback {
	if input == nil {
		return nil
	}

	return &playback.Detection_Robot_Feedback{
		DribblerBallContact:  input.DribblerBallContact,
		KickChargePercentage: input.KickChargePercentage,
		BatteryPercentage:    input.BatteryPercentage,
		Wheels:               robotFeedbackWheelsMapper(input.Wheels),
	}
}

func robotFeedbackWheelsMapper(input []*perception.Robot_Feedback_Wheel) []*playback.Detection_Robot_Feedback_Wheel {
	output := make([]*playback.Detection_Robot_Feedback_Wheel, 0)

	for _, wheel := range input {
		output = append(output, &playback.Detection_Robot_Feedback_Wheel{
			WheelId:    wheel.WheelId,
			MotorSpeed: wheel.MotorSpeed,
		})
	}

	return output
}

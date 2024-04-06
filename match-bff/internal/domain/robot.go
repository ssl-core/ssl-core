package domain

import (
	"github.com/robocin/ssl-core/match-bff/pkg/pb/common"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

type Robot struct {
	Confidence      float32   `json:"confidence"`
	RobotId         uint8     `json:"robot_id"`
	RobotColor      string    `json:"robot_color"`
	Position        []float32 `json:"position"`
	Angle           float32   `json:"angle"`
	Velocity        []float32 `json:"velocity"`
	AngularVelocity float32   `json:"angular_velocity"`
	Radius          float32   `json:"radius"`
	Height          float32   `json:"height"`
	DribblerWidth   float32   `json:"dribbler_width"`
}

func NewRobotFromProto(robot *vision.Robot) Robot {
	return Robot{
		Confidence:      robot.Confidence,
		RobotId:         uint8(robot.RobotId.Number),
		RobotColor:      translateColor(robot.RobotId.Color),
		Position:        []float32{robot.Position.X, robot.Position.Y},
		Angle:           robot.Angle,
		Velocity:        []float32{robot.Velocity.X, robot.Velocity.Y},
		AngularVelocity: robot.AngularVelocity,
		Radius:          robot.PhysicalAttributes.Radius,
		Height:          robot.PhysicalAttributes.Height,
		DribblerWidth:   robot.PhysicalAttributes.DribblerWidth,
	}
}

func translateColor(color common.RobotId_Color) string {
	switch color {
	case common.RobotId_COLOR_YELLOW:
		return "yellow"
	case common.RobotId_COLOR_BLUE:
		return "blue"
	}

	return "unspecified"
}

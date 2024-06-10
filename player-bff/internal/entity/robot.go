package entity

import (
	"github.com/robocin/ssl-core/player-bff/internal/util"
	"github.com/robocin/ssl-core/player-bff/pkg/pb/common"
	"github.com/robocin/ssl-core/player-bff/pkg/pb/vision"
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
	confidence := util.SetDefaultIfNil(robot.Confidence, 0)
	pbRobotId := util.SetDefaultIfNil(robot.RobotId, &common.RobotId{})
	robotId := uint8(util.SetDefaultIfNil(pbRobotId.Number, 0))
	robotColor := translateColor(util.SetDefaultIfNil(pbRobotId.Color, common.RobotId_COLOR_UNSPECIFIED))
	pbPosition := util.SetDefaultIfNil(robot.Position, &common.Point2Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0) / 1000, util.SetDefaultIfNil(pbPosition.Y, 0) / 1000}
	angle := util.SetDefaultIfNil(robot.Angle, 0)
	pbVelocity := util.SetDefaultIfNil(robot.Velocity, &common.Point2Df{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.X, 0) / 1000, util.SetDefaultIfNil(pbVelocity.Y, 0) / 1000}
	angularVelocity := util.SetDefaultIfNil(robot.AngularVelocity, 0)
	pbPhysicalAttributes := util.SetDefaultIfNil(robot.PhysicalAttributes, &vision.Robot_PhysicalAttributes{})
	radius := util.SetDefaultIfNil(pbPhysicalAttributes.Radius, 0) / 1000
	height := util.SetDefaultIfNil(pbPhysicalAttributes.Height, 0) / 1000
	dribblerWidth := util.SetDefaultIfNil(pbPhysicalAttributes.DribblerWidth, 0) / 1000

	return Robot{
		Confidence:      confidence,
		RobotId:         robotId,
		RobotColor:      robotColor,
		Position:        position,
		Angle:           angle,
		Velocity:        velocity,
		AngularVelocity: angularVelocity,
		Radius:          radius,
		Height:          height,
		DribblerWidth:   dribblerWidth,
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

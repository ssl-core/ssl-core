package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

type Robot struct {
	Confidence      float32   `json:"confidence"`
	RobotId         RobotId   `json:"robot_id"`
	Position        []float32 `json:"position"`
	Angle           float32   `json:"angle"`
	Velocity        []float32 `json:"velocity"`
	AngularVelocity float32   `json:"angular_velocity"`
	Radius          float32   `json:"radius"`
	Height          float32   `json:"height"`
	DribblerWidth   float32   `json:"dribbler_width"`
}

func NewRobot(robot_pb *perception.Robot) Robot {
	confidence := util.SetDefaultIfNil(robot_pb.Confidence, 0)
	pbRobotId := util.SetDefaultIfNil(robot_pb.RobotId, &common.RobotId{})
	id := util.SetDefaultIfNil(pbRobotId.Number, 0)
	color := translateRobotColorFromProto(util.SetDefaultIfNil(pbRobotId.Color, common.RobotId_COLOR_UNSPECIFIED))
	robotId := RobotId{Id: id, Color: color}
	pbPosition := util.SetDefaultIfNil(robot_pb.Position, &common.Point2Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0), util.SetDefaultIfNil(pbPosition.Y, 0)}
	angle := util.SetDefaultIfNil(robot_pb.Angle, 0)
	pbVelocity := util.SetDefaultIfNil(robot_pb.Velocity, &common.Point2Df{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.X, 0), util.SetDefaultIfNil(pbVelocity.Y, 0)}
	angularVelocity := util.SetDefaultIfNil(robot_pb.AngularVelocity, 0)
	radius := util.SetDefaultIfNil(robot_pb.Radius, 0)
	height := util.SetDefaultIfNil(robot_pb.Height, 0)
	dribblerWidth := util.SetDefaultIfNil(robot_pb.DribblerWidth, 0)

	return Robot{
		Confidence:      confidence,
		RobotId:         robotId,
		Position:        position,
		Angle:           angle,
		Velocity:        velocity,
		AngularVelocity: angularVelocity,
		Radius:          radius,
		Height:          height,
		DribblerWidth:   dribblerWidth,
	}
}

func translateRobotColorFromProto(color common.RobotId_Color) RobotColor {
	switch color {
	case common.RobotId_COLOR_YELLOW:
		return Yellow
	case common.RobotId_COLOR_BLUE:
		return Blue
	}

	return Unspecified
}

func translateRobotColorToProto(color RobotColor) common.RobotId_Color {
	switch color {
	case Yellow:
		return common.RobotId_COLOR_YELLOW
	case Blue:
		return common.RobotId_COLOR_BLUE
	}

	return common.RobotId_COLOR_UNSPECIFIED
}

func (r *Robot) ToJson() ([]byte, error) {
	return json.Marshal(r)
}

func (r *Robot) ToProto() *playback.Detection_Robot {
	return &playback.Detection_Robot{
		Confidence:      r.Confidence,
		RobotId:         &common.RobotId{Number: uint32(r.RobotId.Id), Color: translateRobotColorToProto(r.RobotId.Color)},
		Position:        &common.Point2Df{X: r.Position[0], Y: r.Position[1]},
		Angle:           r.Angle,
		Velocity:        &common.Point2Df{X: r.Velocity[0], Y: r.Velocity[1]},
		AngularVelocity: r.AngularVelocity,
		Radius:          r.Radius,
		Height:          r.Height,
		DribblerWidth:   r.DribblerWidth,
	}
}

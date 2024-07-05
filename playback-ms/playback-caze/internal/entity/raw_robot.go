package entity

import (
	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

type RawRobot struct {
	RobotId    uint8     `json:"robot_id"`
	RobotColor string    `json:"robot_color"`
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Angle      float32   `json:"angle"`
	Height     float32   `json:"height"`
}

func NewRawRobot(raw_robot_pb *playback.RawDetection_Camera_Robot) RawRobot {
	pbRobotId := util.SetDefaultIfNil(raw_robot_pb.RobotId, &common.RobotId{})
	robotId := uint8(util.SetDefaultIfNil(pbRobotId.Number, 0))
	robotColor := translateRawRobotColor(util.SetDefaultIfNil(pbRobotId.Color, common.RobotId_COLOR_UNSPECIFIED))
	confidence := util.SetDefaultIfNil(raw_robot_pb.Confidence, 0)
	pbPosition := util.SetDefaultIfNil(raw_robot_pb.Position, &common.Point2Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0), util.SetDefaultIfNil(pbPosition.Y, 0)}
	angle := util.SetDefaultIfNil(raw_robot_pb.Angle, 0)
	height := util.SetDefaultIfNil(raw_robot_pb.Height, 0)

	return RawRobot{
		RobotId:    robotId,
		RobotColor: robotColor,
		Confidence: confidence,
		Position:   position,
		Angle:      angle,
		Height:     height,
	}
}

func translateRawRobotColor(color common.RobotId_Color) string {
	switch color {
	case common.RobotId_COLOR_YELLOW:
		return "yellow"
	case common.RobotId_COLOR_BLUE:
		return "blue"
	}

	return "unspecified"
}

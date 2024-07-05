package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/detection"
)

type RawRobot struct {
	RobotId    RobotId   `json:"robot_id"`
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Angle      float32   `json:"angle"`
	Height     float32   `json:"height"`
}

func NewRawRobot(raw_robot_pb *playback.RawDetection_Camera_Robot) RawRobot {
	pbRobotId := util.SetDefaultIfNil(raw_robot_pb.RobotId, &common.RobotId{})
	id := util.SetDefaultIfNil(pbRobotId.Number, 0)
	color := translateRawRobotColor(util.SetDefaultIfNil(pbRobotId.Color, common.RobotId_COLOR_UNSPECIFIED))
	robotId := RobotId{Id: id, Color: color}
	confidence := util.SetDefaultIfNil(raw_robot_pb.Confidence, 0)
	pbPosition := util.SetDefaultIfNil(raw_robot_pb.Position, &common.Point2Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0), util.SetDefaultIfNil(pbPosition.Y, 0)}
	angle := util.SetDefaultIfNil(raw_robot_pb.Angle, 0)
	height := util.SetDefaultIfNil(raw_robot_pb.Height, 0)

	return RawRobot{
		RobotId:    robotId,
		Confidence: confidence,
		Position:   position,
		Angle:      angle,
		Height:     height,
	}
}

func NewRawRobotFromSSLDetectionRobot(detection_robot_pb *detection.SSL_DetectionRobot, color common.RobotId_Color) *RawRobot {
	id := util.SetDefaultIfNil(*detection_robot_pb.RobotId, 0)
	robotId := RobotId{Id: id, Color: translateRawRobotColor(color)}
	confidence := util.SetDefaultIfNil(*detection_robot_pb.Confidence, 0)
	position := []float32{util.SetDefaultIfNil(*detection_robot_pb.X, 0), util.SetDefaultIfNil(*detection_robot_pb.Y, 0)}
	angle := util.SetDefaultIfNil(*detection_robot_pb.Orientation, 0)
	height := util.SetDefaultIfNil(*detection_robot_pb.Height, 0)

	return &RawRobot{
		RobotId:    robotId,
		Confidence: confidence,
		Position:   []float32{position[0], position[1], 0},
		Angle:      angle,
		Height:     height,
	}
}

func translateRawRobotColor(color common.RobotId_Color) RobotColor {
	switch color {
	case common.RobotId_COLOR_YELLOW:
		return Yellow
	case common.RobotId_COLOR_BLUE:
		return Blue
	}

	return Unspecified
}

func (rr *RawRobot) ToJson() ([]byte, error) {
	return json.Marshal(rr)
}

func (rr *RawRobot) ToProto() *playback.RawDetection_Camera_Robot {
	return &playback.RawDetection_Camera_Robot{
		RobotId:    &common.RobotId{Number: uint32(rr.RobotId.Id), Color: translateRawRobotColorToProto(rr.RobotId.Color)},
		Confidence: rr.Confidence,
		Position:   &common.Point2Df{X: rr.Position[0], Y: rr.Position[1]},
		Angle:      rr.Angle,
		Height:     rr.Height,
	}
}

func translateRawRobotColorToProto(color RobotColor) common.RobotId_Color {
	switch color {
	case Yellow:
		return common.RobotId_COLOR_YELLOW
	case Blue:
		return common.RobotId_COLOR_BLUE
	}

	return common.RobotId_COLOR_UNSPECIFIED
}

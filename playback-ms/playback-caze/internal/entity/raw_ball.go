package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/detection"
)

type RawBall struct {
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
}

func NewRawBall(raw_ball_pb *playback.RawDetection_Camera_Ball) RawBall {
	confidence := util.SetDefaultIfNil(raw_ball_pb.Confidence, 0)
	pbPosition := util.SetDefaultIfNil(raw_ball_pb.Position, &common.Point3Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0), util.SetDefaultIfNil(pbPosition.Y, 0), util.SetDefaultIfNil(pbPosition.Z, 0)}

	return RawBall{
		Confidence: confidence,
		Position:   position,
	}
}

func NewRawBallFromSSLDetectionBall(detection_ball_pb *detection.SSL_DetectionBall) *RawBall {
	confidence := util.SetDefaultIfNil(*detection_ball_pb.Confidence, 0)
	position := []float32{util.SetDefaultIfNil(*detection_ball_pb.X, 0), util.SetDefaultIfNil(*detection_ball_pb.Y, 0), util.SetDefaultIfNil(*detection_ball_pb.Z, 0)}

	return &RawBall{
		Confidence: confidence,
		Position:   []float32{position[0], position[1], 0},
	}
}

func (rb *RawBall) ToJson() ([]byte, error) {
	return json.Marshal(rb)
}

func (rb *RawBall) ToProto() *playback.RawDetection_Camera_Ball {
	return &playback.RawDetection_Camera_Ball{
		Confidence: rb.Confidence,
		Position: &common.Point3Df{
			X: rb.Position[0],
			Y: rb.Position[1],
			Z: rb.Position[2],
		},
	}
}

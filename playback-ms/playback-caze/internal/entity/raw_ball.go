package entity

import (
	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
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

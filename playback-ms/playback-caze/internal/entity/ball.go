package entity

import (
	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

type Ball struct {
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Velocity   []float32 `json:"velocity"`
}

func NewBall(ball_pb *perception.Ball) Ball {
	confidence := util.SetDefaultIfNil(ball_pb.Confidence, 0)
	pbPosition := util.SetDefaultIfNil(ball_pb.Position, &common.Point3Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0), util.SetDefaultIfNil(pbPosition.Y, 0), util.SetDefaultIfNil(pbPosition.Z, 0)}
	pbVelocity := util.SetDefaultIfNil(ball_pb.Velocity, &common.Point3Df{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.X, 0), util.SetDefaultIfNil(pbVelocity.Y, 0), util.SetDefaultIfNil(pbVelocity.Z, 0)}

	return Ball{
		Confidence: confidence,
		Position:   position,
		Velocity:   velocity,
	}
}

func (b *Ball) ToProto() *playback.Detection_Ball {
	return &playback.Detection_Ball{
		Confidence: b.Confidence,
		Position: &common.Point3Df{
			X: b.Position[0],
			Y: b.Position[1],
			Z: b.Position[2],
		},
		Velocity: &common.Point3Df{
			X: b.Velocity[0],
			Y: b.Velocity[1],
			Z: b.Velocity[2],
		},
	}
}

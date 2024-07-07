package entity

import (
	"github.com/robocin/ssl-core/player-bff/internal/util"
	"github.com/robocin/ssl-core/player-bff/pkg/pb/common"
	"github.com/robocin/ssl-core/player-bff/pkg/pb/playback"
)

type Ball struct {
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Velocity   []float32 `json:"velocity"`
}

func NewBall(ball *playback.Detection_Ball) Ball {
	confidence := util.SetDefaultIfNil(ball.Confidence, 0)
	pbPosition := util.SetDefaultIfNil(ball.Position, &common.Point3Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.X, 0) / 1000, util.SetDefaultIfNil(pbPosition.Y, 0) / 1000, util.SetDefaultIfNil(pbPosition.Z, 0) / 1000}
	pbVelocity := util.SetDefaultIfNil(ball.Velocity, &common.Point3Df{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.X, 0), util.SetDefaultIfNil(pbVelocity.Y, 0), util.SetDefaultIfNil(pbVelocity.Z, 0)}

	return Ball{
		Confidence: confidence,
		Position:   position,
		Velocity:   velocity,
	}
}

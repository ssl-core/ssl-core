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
	confidence := util.SetDefaultIfNil(ball.GetConfidence(), 0)
	pbPosition := util.SetDefaultIfNil(ball.GetPosition(), &common.Point3Df{})
	position := []float32{util.SetDefaultIfNil(pbPosition.GetX(), 0) / 1000, util.SetDefaultIfNil(pbPosition.GetY(), 0) / 1000, util.SetDefaultIfNil(pbPosition.GetZ(), 0) / 1000}
	pbVelocity := util.SetDefaultIfNil(ball.GetVelocity(), &common.Point3Df{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.GetX(), 0), util.SetDefaultIfNil(pbVelocity.GetY(), 0), util.SetDefaultIfNil(pbVelocity.GetZ(), 0)}

	return Ball{
		Confidence: confidence,
		Position:   position,
		Velocity:   velocity,
	}
}

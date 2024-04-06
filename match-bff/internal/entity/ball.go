package entity

import "github.com/robocin/ssl-core/match-bff/pkg/pb/vision"

type Ball struct {
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Velocity   []float32 `json:"velocity"`
}

func NewBallFromProto(ball *vision.Ball) Ball {
	return Ball{
		Confidence: ball.Confidence,
		Position:   []float32{ball.Position.X, ball.Position.Y},
		Velocity:   []float32{ball.Velocity.X, ball.Velocity.Y},
	}
}

package domain

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

type Frame struct {
	SerialId          uint64    `json:"serial_id"`
	EffectiveSerialId uint64    `json:"effective_serial_id"`
	Fps               float32   `json:"fps"`
	Balls             []Ball    `json:"balls"`
	Robots            []Robot   `json:"robots"`
	Field             Field     `json:"field"`
	CreatedAt         time.Time `json:"created_at"`
}

func NewFrameFromProto(frame *vision.Frame) Frame {
	field := NewFieldFromProto(frame.Field)

	balls := make([]Ball, len(frame.Balls))
	for i, ball := range frame.Balls {
		balls[i] = NewBallFromProto(ball)
	}

	robots := make([]Robot, len(frame.Robots))
	for i, robot := range frame.Robots {
		robots[i] = NewRobotFromProto(robot)
	}

	return Frame{
		SerialId:          frame.Properties.SerialId,
		EffectiveSerialId: frame.Properties.EffectiveSerialId,
		Fps:               frame.Properties.Fps,
		Balls:             balls,
		Robots:            robots,
		Field:             field,
		CreatedAt:         frame.Properties.CreatedAt.AsTime(),
	}
}

func (f *Frame) ToJson() ([]byte, error) {
	return json.Marshal(f)
}

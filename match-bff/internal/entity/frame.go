package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/match-bff/internal/util"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
	"google.golang.org/protobuf/types/known/timestamppb"
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
	field := NewFieldFromProto(util.SetDefaultIfNil(frame.Field, &vision.Field{}))

	balls := make([]Ball, len(frame.Balls))
	for i, ball := range frame.Balls {
		balls[i] = NewBallFromProto(util.SetDefaultIfNil(ball, &vision.Ball{}))
	}

	robots := make([]Robot, len(frame.Robots))
	for i, robot := range frame.Robots {
		robots[i] = NewRobotFromProto(util.SetDefaultIfNil(robot, &vision.Robot{}))
	}

	pbProperties := util.SetDefaultIfNil(frame.Properties, &vision.Frame_Properties{})
	serialId := util.SetDefaultIfNil(pbProperties.SerialId, 0)
	effectiveSerialId := util.SetDefaultIfNil(pbProperties.EffectiveSerialId, 0)
	fps := util.SetDefaultIfNil(pbProperties.Fps, 0)
	createdAt := util.SetDefaultIfNil(pbProperties.CreatedAt, &timestamppb.Timestamp{}).AsTime()

	return Frame{
		SerialId:          serialId,
		EffectiveSerialId: effectiveSerialId,
		Fps:               fps,
		Balls:             balls,
		Robots:            robots,
		Field:             field,
		CreatedAt:         createdAt,
	}
}

func (f *Frame) ToJson() ([]byte, error) {
	return json.Marshal(f)
}

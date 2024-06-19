package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/player-bff/internal/util"
	"github.com/robocin/ssl-core/player-bff/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Frame struct {
	StartTime   time.Time `json:"start_time"`
	CurrentTime time.Time `json:"current_time"`
	SerialId    uint64    `json:"serial_id"`
	Fps         uint32    `json:"fps"`
	Balls       []Ball    `json:"balls"`
	Robots      []Robot   `json:"robots"`
	Field       Field     `json:"field"`
}

func NewFrame(sample *playback.Sample) Frame {
	field := NewField(util.SetDefaultIfNil(sample.Field, &playback.Field{}))

	balls := make([]Ball, len(sample.Detection.Balls))
	for i, ball := range sample.Detection.Balls {
		balls[i] = NewBall(util.SetDefaultIfNil(ball, &playback.Detection_Ball{}))
	}

	robots := make([]Robot, len(sample.Detection.Robots))
	for i, robot := range sample.Detection.Robots {
		robots[i] = NewRobot(util.SetDefaultIfNil(robot, &playback.Detection_Robot{}))
	}

	serialId := util.SetDefaultIfNil(sample.Detection.SerialId, 0)
	fps := util.SetDefaultIfNil(sample.Detection.ExpectedFramerate, 0)
	currentTime := util.SetDefaultIfNil(sample.Timestamp, &timestamppb.Timestamp{}).AsTime()
	startTime := util.SetDefaultIfNil(sample.FirstTimestamp, &timestamppb.Timestamp{}).AsTime()

	return Frame{
		StartTime:   startTime,
		CurrentTime: currentTime,
		SerialId:    serialId,
		Fps:         fps,
		Balls:       balls,
		Robots:      robots,
		Field:       field,
	}
}

func (f *Frame) ToJson() ([]byte, error) {
	return json.Marshal(f)
}

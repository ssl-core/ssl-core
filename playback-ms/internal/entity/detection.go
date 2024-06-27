package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/reflect/protoreflect"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Detection struct {
	SerialId  uint64    `json:"serial_id"`
	CreatedAt time.Time `json:"created_at"`
	Fps       uint32    `json:"fps"`
	Balls     []Ball    `json:"balls"`
	Robots    []Robot   `json:"robots"`
}

// ProtoReflect implements protoreflect.ProtoMessage.
func (d *Detection) ProtoReflect() protoreflect.Message {
	panic("unimplemented")
}

func NewDetection(detection *playback.Detection) Detection {
	balls := make([]Ball, len(detection.Balls))
	for i, ball := range detection.Balls {
		balls[i] = NewBall(util.SetDefaultIfNil(ball, &playback.Detection_Ball{}))
	}

	robots := make([]Robot, len(detection.Robots))
	for i, robot := range detection.Robots {
		robots[i] = NewRobot(util.SetDefaultIfNil(robot, &playback.Detection_Robot{}))
	}

	serialId := util.SetDefaultIfNil(detection.SerialId, 0)
	fps := util.SetDefaultIfNil(detection.ExpectedFramerate, 0)
	created_at := util.SetDefaultIfNil(detection.CreatedAt, &timestamppb.Timestamp{}).AsTime()

	return Detection{
		SerialId:  serialId,
		CreatedAt: created_at,
		Fps:       fps,
		Balls:     balls,
		Robots:    robots,
	}
}

func (d *Detection) ToJson() ([]byte, error) {
	return json.Marshal(d)
}

func NewDetectionFromString(protobuf string) (Detection, error) {
	var detection Detection
	err := json.Unmarshal([]byte(protobuf), &detection)
	if err != nil {
		return Detection{}, err
	}
	return detection, nil
}

func (d *Detection) ToProto() *playback.Detection {
	balls := make([]*playback.Detection_Ball, len(d.Balls))
	for i, ball := range d.Balls {
		balls[i] = ball.ToProto()
	}

	robots := make([]*playback.Detection_Robot, len(d.Robots))
	for i, robot := range d.Robots {
		robots[i] = robot.ToProto()
	}

	return &playback.Detection{
		SerialId:          d.SerialId,
		CreatedAt:         timestamppb.New(d.CreatedAt),
		ExpectedFramerate: d.Fps,
		Balls:             balls,
		Robots:            robots,
	}
}

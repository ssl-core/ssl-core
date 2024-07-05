package entity

import (
	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

type RawCamera struct {
	CameraId uint32     `json:"camera_id"`
	SerialId uint32     `json:"serial_id"`
	Balls    []RawBall  `json:"balls"`
	Robots   []RawRobot `json:"robots"`
}

func NewRawCamera(raw_camera_pb *playback.RawDetection_Camera) RawCamera {
	balls := make([]RawBall, len(raw_camera_pb.Balls))
	for i, raw_ball_pb := range raw_camera_pb.Balls {
		balls[i] = NewRawBall(raw_ball_pb)
	}

	robots := make([]RawRobot, len(raw_camera_pb.Robots))
	for i, raw_robot_pb := range raw_camera_pb.Robots {
		robots[i] = NewRawRobot(raw_robot_pb)
	}

	cameraId := util.SetDefaultIfNil(raw_camera_pb.CameraId, 0)
	serialId := util.SetDefaultIfNil(raw_camera_pb.SerialId, 0)

	return RawCamera{
		CameraId: cameraId,
		SerialId: serialId,
		Balls:    balls,
		Robots:   robots,
	}
}

package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/detection"
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

func NewRawCameraFromSSLDetectionFrame(detection_frame_pb *detection.SSL_DetectionFrame) *RawCamera {
	cameraId := util.SetDefaultIfNil(*detection_frame_pb.CameraId, 0)
	serialId := util.SetDefaultIfNil(*detection_frame_pb.FrameNumber, 0)

	balls := make([]RawBall, len(detection_frame_pb.Balls))
	for i, detection_ball_pb := range detection_frame_pb.Balls {
		balls[i] = *NewRawBallFromSSLDetectionBall(detection_ball_pb)
	}

	robots := make([]RawRobot, len(detection_frame_pb.RobotsYellow)+len(detection_frame_pb.RobotsBlue))
	for i, detection_robot_pb := range detection_frame_pb.RobotsYellow {
		robots[i] = *NewRawRobotFromSSLDetectionRobot(detection_robot_pb, common.RobotId_Color(Yellow))
	}
	for i, detection_robot_pb := range detection_frame_pb.RobotsBlue {
		robots[i] = *NewRawRobotFromSSLDetectionRobot(detection_robot_pb, common.RobotId_Color(Blue))
	}

	return &RawCamera{
		CameraId: cameraId,
		SerialId: serialId,
		Balls:    balls,
		Robots:   robots,
	}
}

func (rc *RawCamera) ToJson() ([]byte, error) {
	return json.Marshal(rc)
}

func (rc *RawCamera) ToProto() *playback.RawDetection_Camera {
	balls := make([]*playback.RawDetection_Camera_Ball, len(rc.Balls))
	for i, ball := range rc.Balls {
		balls[i] = ball.ToProto()
	}

	robots := make([]*playback.RawDetection_Camera_Robot, len(rc.Robots))
	for i, robot := range rc.Robots {
		robots[i] = robot.ToProto()
	}

	return &playback.RawDetection_Camera{
		CameraId: rc.CameraId,
		SerialId: rc.SerialId,
		Balls:    balls,
		Robots:   robots,
	}
}

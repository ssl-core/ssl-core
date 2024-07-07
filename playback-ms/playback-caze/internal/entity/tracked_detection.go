package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/game_controller"
)

type TrackedDetection struct {
	SourceUniqueId     string         `json:"source_unique_id"`
	SourceSoftwareName string         `json:"source_software_name"`
	SerialId           uint32         `json:"serial_id"`
	Balls              []TrackedBall  `json:"balls"`
	Robots             []TrackedRobot `json:"robots"`
}

func NewTrackedDetectionFromTrackedPackets(tracked_packets_pb []*game_controller.TrackerWrapperPacket) *TrackedDetection {
	if len(tracked_packets_pb) == 0 {
		return nil
	}
	tracked_packet_pb := (tracked_packets_pb)[len(tracked_packets_pb)-1]
	tracked_detection_pb := tracked_packet_pb.TrackedFrame

	balls := make([]TrackedBall, len(tracked_detection_pb.Balls))
	for i, tracked_ball_pb := range tracked_detection_pb.Balls {
		balls[i] = *NewTrackedBallFromTracked(tracked_ball_pb)
	}

	robots := make([]TrackedRobot, len(tracked_detection_pb.Robots))
	for i, tracked_robot_pb := range tracked_detection_pb.Robots {
		robots[i] = *NewTrackedRobotFromTracked(tracked_robot_pb)
	}

	sourceUniqueId := util.SetDefaultIfNil(*tracked_packet_pb.Uuid, "NO-UUID")
	sourceSoftwareName := util.SetDefaultIfNil(*tracked_packet_pb.SourceName, "UNKONWN")
	serialId := util.SetDefaultIfNil(*tracked_detection_pb.FrameNumber, 0)

	return &TrackedDetection{
		SourceUniqueId:     sourceUniqueId,
		SourceSoftwareName: sourceSoftwareName,
		SerialId:           serialId,
		Balls:              balls,
		Robots:             robots,
	}
}

func (td *TrackedDetection) ToJson() ([]byte, error) {
	return json.Marshal(td)
}

func (td *TrackedDetection) ToProto() *playback.TrackedDetection {
	sourceUniqueId := td.SourceUniqueId
	sourceSoftwareName := td.SourceSoftwareName
	serialId := &td.SerialId

	balls := make([]*playback.TrackedDetection_Ball, len(td.Balls))
	for i, tracked_ball := range td.Balls {
		balls[i] = tracked_ball.ToProto()
	}

	robots := make([]*playback.TrackedDetection_Robot, len(td.Robots))
	for i, tracked_robot := range td.Robots {
		robots[i] = tracked_robot.ToProto()
	}

	return &playback.TrackedDetection{
		SourceUniqueId:     sourceUniqueId,
		SourceSoftwareName: sourceSoftwareName,
		SerialId:           *serialId,
		Balls:              balls,
		Robots:             robots,
	}
}

package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/game_controller"
)

type TrackedRobot struct {
	Confidence      float32   `json:"confidence"`
	RobotId         uint32    `json:"robot_id"`
	Team            string    `json:"team"`
	Position        []float32 `json:"position"`
	Angle           float32   `json:"angle"`
	Velocity        []float32 `json:"velocity"`
	AngularVelocity float32   `json:"angular_velocity"`
}

func NewTrackedRobotFromTracked(tracked_robot_pb *game_controller.TrackedRobot) *TrackedRobot {
	confidence := util.SetDefaultIfNil(tracked_robot_pb.GetVisibility(), 0.0)
	robotId := util.SetDefaultIfNil(tracked_robot_pb.RobotId.GetId(), 0)
	robotTeam := util.SetDefaultIfNil(tracked_robot_pb.RobotId.Team.String(), "UNKNOWN")
	pbPosition := util.SetDefaultIfNil(tracked_robot_pb.Pos, &game_controller.Vector2{})
	position := []float32{util.SetDefaultIfNil(pbPosition.GetX(), 0), util.SetDefaultIfNil(pbPosition.GetY(), 0)}
	angle := util.SetDefaultIfNil(tracked_robot_pb.GetOrientation(), 0)
	pbVelocity := util.SetDefaultIfNil(tracked_robot_pb.Vel, &game_controller.Vector2{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.GetX(), 0), util.SetDefaultIfNil(pbVelocity.GetY(), 0)}
	angularVelocity := util.SetDefaultIfNil(tracked_robot_pb.GetVelAngular(), 0)

	return &TrackedRobot{
		Confidence:      confidence,
		RobotId:         robotId,
		Team:            robotTeam,
		Position:        position,
		Angle:           angle,
		Velocity:        velocity,
		AngularVelocity: angularVelocity,
	}
}

func (tr *TrackedRobot) ToJson() ([]byte, error) {
	return json.Marshal(tr)
}

func (tr *TrackedRobot) ToProto() *playback.TrackedDetection_Robot {
	return &playback.TrackedDetection_Robot{
		Confidence: tr.Confidence,
		RobotId: &common.RobotId{
			Number: tr.RobotId,
			Color:  translateRobotTeamToProto(tr.Team),
		},
		Position: &common.Point2Df{
			X: tr.Position[0],
			Y: tr.Position[1],
		},
		Angle:           tr.Angle,
		Velocity:        &common.Point2Df{X: tr.Velocity[0], Y: tr.Velocity[1]},
		AngularVelocity: tr.AngularVelocity,
	}
}

func translateRobotTeamToProto(team string) common.RobotId_Color {
	switch team {
	case "YELLOW":
		return common.RobotId_COLOR_YELLOW
	case "BLUE":
		return common.RobotId_COLOR_BLUE
	}

	return common.RobotId_COLOR_UNSPECIFIED
}

package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/internal/util"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/game_controller"
)

type TrackedBall struct {
	Confidence float32   `json:"confidence"`
	Position   []float32 `json:"position"`
	Velocity   []float32 `json:"velocity"`
	// KickInformation *KickInformation `json:"kick_information"`
}

// TODO(matheusvtna): Add kick information to the tracked ball. How to convert KickedBalls to KickInformation?
// type KickInformation struct {
// 	RobotID                uint8     `json:"robot_id"`
// 	RobotColor             string    `json:"robot_color"`
// 	StartPosition          []float32 `json:"start_position"`
// 	StartVelocity          []float32 `json:"start_velocity"`
// 	StartTimestamp         string    `json:"start_timestamp"`
// 	PredictedStopPosition  []float32 `json:"predicted_stop_position"`
// 	PredictedStopTimestamp string    `json:"predicted_stop_timestamp"`
// }

func NewTrackedBallFromTracked(tracked_ball_pb *game_controller.TrackedBall) *TrackedBall {
	confidence := util.SetDefaultIfNil(tracked_ball_pb.GetVisibility(), 0.0)
	pbPosition := util.SetDefaultIfNil(tracked_ball_pb.Pos, &game_controller.Vector3{})
	position := []float32{util.SetDefaultIfNil(pbPosition.GetX(), 0), util.SetDefaultIfNil(pbPosition.GetY(), 0), util.SetDefaultIfNil(pbPosition.GetZ(), 0)}
	pbVelocity := util.SetDefaultIfNil(tracked_ball_pb.Vel, &game_controller.Vector3{})
	velocity := []float32{util.SetDefaultIfNil(pbVelocity.GetX(), 0), util.SetDefaultIfNil(pbVelocity.GetY(), 0), util.SetDefaultIfNil(pbVelocity.GetZ(), 0)}

	return &TrackedBall{
		Confidence: confidence,
		Position:   position,
		Velocity:   velocity,
	}
}

func (tb *TrackedBall) ToJson() ([]byte, error) {
	return json.Marshal(tb)
}

func (tb *TrackedBall) ToProto() *playback.TrackedDetection_Ball {
	return &playback.TrackedDetection_Ball{
		Confidence: tb.Confidence,
		Position: &common.Point3Df{
			X: tb.Position[0],
			Y: tb.Position[1],
			Z: tb.Position[2],
		},
		Velocity: &common.Point3Df{
			X: tb.Velocity[0],
			Y: tb.Velocity[1],
			Z: tb.Velocity[2],
		},
	}
}

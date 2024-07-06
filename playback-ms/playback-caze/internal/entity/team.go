package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/types/known/durationpb"
)

type Team struct {
	Name                                   string          `json:"name"`
	Score                                  uint32          `json:"score"`
	RobotIds                               []RobotId       `json:"robot_ids"`
	GoalkeeperId                           RobotId         `json:"goalkeeper_id"`
	YellowCards                            uint32          `json:"yellow_cards"`
	TimeToExpireActiveYellowCards          []time.Duration `json:"time_to_expire_active_yellow_cards"`
	RedCards                               uint32          `json:"red_cards"`
	TimeoutsLeft                           uint32          `json:"timeouts_left"`
	TotalTimeoutTimeLeft                   time.Duration   `json:"total_timeout_time_left"`
	TotalFoulsCommitted                    uint32          `json:"total_fouls_committed"`
	ConsecutiveBallPlacementFailures       uint32          `json:"consecutive_ball_placement_failures"`
	IsBallPlacementEnabled                 bool            `json:"is_ball_placement_enabled"`
	HasBallPlacementFailuresReachedMaximum bool            `json:"has_ball_placement_failures_reached_maximum"`
	MaximumAllowedRobots                   uint32          `json:"maximum_allowed_robots"`
	IsRobotSubstitutionRequested           bool            `json:"is_robot_substitution_requested"`
	IsRobotSubstitutionAllowed             bool            `json:"is_robot_substitution_allowed"`
	RobotSubstitutionsLeft                 uint32          `json:"robot_substitutions_left"`
	RobotSubstitutionTimeLeft              time.Duration   `json:"robot_substitution_time_left"`
}

func (t *Team) ToJson() ([]byte, error) {
	return json.Marshal(t)
}

func NewTeamFromRefereeTeam(team_pb *referee.GameStatus_Team) *Team {
	return &Team{
		Name:                                   team_pb.Name,
		Score:                                  team_pb.Score,
		RobotIds:                               newRobotIdsFromRefereeRobotIds(team_pb.RobotIds),
		GoalkeeperId:                           NewRobotIdFromCommonRobotId(team_pb.GoalkeeperId),
		YellowCards:                            team_pb.YellowCards,
		TimeToExpireActiveYellowCards:          translateTimeDurations(team_pb.TimeToExpireActiveYellowCards),
		RedCards:                               team_pb.RedCards,
		TimeoutsLeft:                           team_pb.TimeoutsLeft,
		TotalTimeoutTimeLeft:                   team_pb.TotalTimeoutTimeLeft.AsDuration(),
		TotalFoulsCommitted:                    team_pb.TotalFoulsCommitted,
		ConsecutiveBallPlacementFailures:       team_pb.ConsecutiveBallPlacementFailures,
		IsBallPlacementEnabled:                 team_pb.IsBallPlacementEnabled,
		HasBallPlacementFailuresReachedMaximum: team_pb.HasBallPlacementFailuresReachedMaximum,
		MaximumAllowedRobots:                   team_pb.MaximumAllowedRobots,
		IsRobotSubstitutionRequested:           team_pb.IsRobotSubstitutionRequested,
		IsRobotSubstitutionAllowed:             team_pb.IsRobotSubstitutionAllowed,
		RobotSubstitutionsLeft:                 team_pb.RobotSubstitutionsLeft,
		RobotSubstitutionTimeLeft:              team_pb.RobotSubstitutionTimeLeft.AsDuration(),
	}
}

func newRobotIdsFromRefereeRobotIds(robotIdsPb []*common.RobotId) []RobotId {
	robotIds := make([]RobotId, len(robotIdsPb))
	for i, robotIdPb := range robotIdsPb {
		robotIds[i] = NewRobotIdFromCommonRobotId(robotIdPb)
	}
	return robotIds
}

func translateTimeDurations(timeDurationsPb []*durationpb.Duration) []time.Duration {
	translatedTimeDurations := make([]time.Duration, len(timeDurationsPb))
	for i, timeDurationPb := range timeDurationsPb {
		translatedTimeDurations[i] = timeDurationPb.AsDuration()
	}
	return translatedTimeDurations
}

package entity

import (
	"encoding/json"
	"time"
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

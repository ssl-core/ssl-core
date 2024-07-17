package entity

import (
	"time"

	"github.com/robocin/ssl-core/protocols/playback"
)

type Team struct {
	Name                                   string          `json:"name"`
	Score                                  uint32          `json:"score"`
	RobotColor                             string          `json:"robot_color"`
	GoalkeeperId                           uint32          `json:"goalkeeper_id"`
	IsPositiveHalf                         bool            `json:"is_positive_half"`
	YellowCards                            uint32          `json:"yellow_cards"`
	TimeToExpireActiveYellowCards          []time.Duration `json:"time_to_expire_active_yellow_cards"`
	RedCards                               uint32          `json:"red_cards"`
	TimeoutsLeft                           uint32          `json:"timeouts_left"`
	TotalTimeoutTimeLeft                   time.Duration   `json:"total_timeout_time_left"`
	FoulsCommited                          uint32          `json:"fouls_committed"`
	ConsecutiveBallPlacementFailures       uint32          `json:"consecutive_ball_placement_failures"`
	IsBallPlacementEnabled                 bool            `json:"is_ball_placement_enabled"`
	HasBallPlacementFailuresReachedMaximum bool            `json:"has_ball_placement_failures_reached_maximum"`
	MaximumAllowedRobots                   uint32          `json:"maximum_allowed_robots"`
	IsRobotSubstitutionRequested           bool            `json:"is_robot_substitution_requested"`
	IsRobotSubstitutionAllowed             bool            `json:"is_robot_substitution_allowed"`
	RobotSubstitutionsLeft                 uint32          `json:"robot_substitutions_left"`
	RobotSubstitutionTimeLeft              time.Duration   `json:"robot_substitution_time_left"`
}

func NewTeam(team *playback.GameStatus_Team, positiveHalf bool) Team {
	color := "yellow"
	if len(team.GetRobotIds()) > 0 {
		color = translateColor(team.GetRobotIds()[0].GetColor())
	}

	times := make([]time.Duration, 0)
	for _, card := range team.GetTimeToExpireActiveYellowCards() {
		times = append(times, card.AsDuration())
	}

	return Team{
		Name:                                   team.GetName(),
		Score:                                  team.GetScore(),
		IsPositiveHalf:                         positiveHalf,
		RobotColor:                             color,
		GoalkeeperId:                           team.GetGoalkeeperId().GetNumber(),
		YellowCards:                            team.GetYellowCards(),
		TimeToExpireActiveYellowCards:          times,
		RedCards:                               team.GetRedCards(),
		TimeoutsLeft:                           team.GetTimeoutsLeft(),
		TotalTimeoutTimeLeft:                   team.GetTotalTimeoutTimeLeft().AsDuration(),
		FoulsCommited:                          team.GetTotalFoulsCommitted(),
		ConsecutiveBallPlacementFailures:       team.GetConsecutiveBallPlacementFailures(),
		IsBallPlacementEnabled:                 team.GetIsBallPlacementEnabled(),
		HasBallPlacementFailuresReachedMaximum: team.GetHasBallPlacementFailuresReachedMaximum(),
		MaximumAllowedRobots:                   team.GetMaximumAllowedRobots(),
		IsRobotSubstitutionRequested:           team.GetIsRobotSubstitutionRequested(),
		IsRobotSubstitutionAllowed:             team.GetIsRobotSubstitutionAllowed(),
		RobotSubstitutionsLeft:                 team.GetRobotSubstitutionsLeft(),
		RobotSubstitutionTimeLeft:              team.GetRobotSubstitutionTimeLeft().AsDuration(),
	}
}

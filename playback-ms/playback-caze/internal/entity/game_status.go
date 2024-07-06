package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
)

type GameStatus struct {
	SourceId               string               `json:"source_id"`
	Description            string               `json:"description"`
	Timestamp              time.Time            `json:"timestamp"`
	MatchType              string               `json:"match_type"`
	HomeTeam               Team                 `json:"home_team"`
	AwayTeam               Team                 `json:"away_team"`
	GameStage              string               `json:"game_stage"`
	GameStageTimeLeft      time.Duration        `json:"game_stage_time_left"`
	TotalCommandsIssued    uint64               `json:"total_commands_issued"`
	CommandIssuedTimestamp time.Time            `json:"command_issued_timestamp"`
	Command                string               `json:"command"`
	NextCommand            string               `json:"next_command"`
	GameEvents             []*GameEvent         `json:"game_events"`
	GameEventsProposals    []GameEventsProposal `json:"game_events_proposals"`
}

func (gs *GameStatus) ToJson() ([]byte, error) {
	return json.Marshal(gs)
}

func NewGameStatusFromRefereeGameStatus(game_status_pb *referee.GameStatus) *GameStatus {
	return &GameStatus{
		SourceId:               game_status_pb.SourceId,
		Description:            *game_status_pb.Description,
		Timestamp:              game_status_pb.Timestamp.AsTime(),
		MatchType:              game_status_pb.MatchType.String(),
		HomeTeam:               *NewTeamFromRefereeTeam(game_status_pb.HomeTeam),
		AwayTeam:               *NewTeamFromRefereeTeam(game_status_pb.AwayTeam),
		GameStage:              game_status_pb.GameStage.String(),
		GameStageTimeLeft:      game_status_pb.GameStageTimeLeft.AsDuration(),
		TotalCommandsIssued:    game_status_pb.TotalCommandsIssued,
		CommandIssuedTimestamp: game_status_pb.CommandIssuedTimestamp.AsTime(),
		Command:                game_status_pb.Command.String(),
		NextCommand:            game_status_pb.NextCommand.String(),
		GameEvents:             game_status_pb.GameEvents,
		GameEventsProposals:    NewGameEventsProposalsFromRefereeGameEventsProposals(game_status_pb.GameEventsProposals),
	}
}

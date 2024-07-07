package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
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
	Command                common.GameCommand   `json:"command"`
	NextCommand            common.GameCommand   `json:"next_command"`
	GameEvents             []*GameEvent         `json:"game_events"`
	GameEventsProposals    []GameEventsProposal `json:"game_events_proposals"`
}

func (gs *GameStatus) ToJson() ([]byte, error) {
	return json.Marshal(gs)
}

// func NewGameStatusFromRefereeGameStatus(game_status_pb *referee.GameStatus) *GameStatus {
// 	command := game_status_pb.Command
// 	nextCommand := game_status_pb.NextCommand

// 	return &GameStatus{
// 		SourceId:               game_status_pb.SourceId,
// 		Description:            *game_status_pb.Description,
// 		Timestamp:              game_status_pb.Timestamp.AsTime(),
// 		MatchType:              game_status_pb.MatchType.String(),
// 		HomeTeam:               *NewTeamFromRefereeTeam(game_status_pb.HomeTeam),
// 		AwayTeam:               *NewTeamFromRefereeTeam(game_status_pb.AwayTeam),
// 		GameStage:              game_status_pb.GameStage.String(),
// 		GameStageTimeLeft:      game_status_pb.GameStageTimeLeft.AsDuration(),
// 		TotalCommandsIssued:    game_status_pb.TotalCommandsIssued,
// 		CommandIssuedTimestamp: game_status_pb.CommandIssuedTimestamp.AsTime(),
// 		Command:                command,
// 		NextCommand:            nextCommand,
// 		GameEvents:             game_status_pb.GameEvents,
// 		GameEventsProposals:    NewGameEventsProposalsFromRefereeGameEventsProposals(game_status_pb.GameEventsProposals),
// 	}
// }

// func (gs *GameStatus) ToProto() *referee.GameStatus {
// 	timestamp := timestamppb.New(gs.Timestamp)
// 	game_stage_time_left := durationpb.New(gs.GameStageTimeLeft)
// 	command_issued_timestamp := timestamppb.New(gs.CommandIssuedTimestamp)
// 	command := gs.Command
// 	next_command := referee.GameCommand_GameCommandType(referee.GameCommand_GameCommandType_value[gs.NextCommand])
// 	game_events := gs.GameEvents
// 	game_events_proposals := NewRefereeGameEventsProposalsFromGameEventsProposals(gs.GameEventsProposals)

// 	return &referee.GameStatus{
// 		SourceId:               gs.SourceId,
// 		Description:            &gs.Description,
// 		Timestamp:              timestamp,
// 		MatchType:              referee.MatchType(referee.MatchType_value[gs.MatchType]),
// 		HomeTeam:               gs.HomeTeam.ToProto(),
// 		AwayTeam:               gs.AwayTeam.ToProto(),
// 		GameStage:              referee.GameStage(referee.GameStage_value[gs.GameStage]),
// 		GameStageTimeLeft:      game_stage_time_left,
// 		TotalCommandsIssued:    gs.TotalCommandsIssued,
// 		CommandIssuedTimestamp: command_issued_timestamp,
// 		Command:                &referee.GameCommand{Command: command},
// 		NextCommand:            &referee.GameCommand{Command: next_command},
// 		GameEvents:             game_events,
// 		GameEventsProposals:    game_events_proposals,
// 	}
// }

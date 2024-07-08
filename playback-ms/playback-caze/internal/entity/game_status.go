package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/types/known/durationpb"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type GameStatus struct {
	SourceId               string               `json:"source_id"`
	Description            string               `json:"description"`
	Timestamp              time.Time            `json:"timestamp"`
	MatchType              *common.MatchType    `json:"match_type"`
	HomeTeam               Team                 `json:"home_team"`
	AwayTeam               Team                 `json:"away_team"`
	GameStage              *common.GameStage    `json:"game_stage"`
	GameStageTimeLeft      time.Duration        `json:"game_stage_time_left"`
	TotalCommandsIssued    uint64               `json:"total_commands_issued"`
	CommandIssuedTimestamp time.Time            `json:"command_issued_timestamp"`
	Command                *common.GameCommand  `json:"command"`
	NextCommand            *common.GameCommand  `json:"next_command"`
	GameEvents             []*GameEvent         `json:"game_events"`
	GameEventsProposals    []GameEventsProposal `json:"game_events_proposals"`
}

func (gs *GameStatus) ToJson() ([]byte, error) {
	return json.Marshal(gs)
}

func NewGameStatusFromRefereeGameStatus(game_status_pb *referee.GameStatus) *GameStatus {
	command := game_status_pb.Command
	nextCommand := game_status_pb.NextCommand

	return &GameStatus{
		SourceId:               game_status_pb.SourceId,
		Description:            game_status_pb.GetDescription(),
		Timestamp:              game_status_pb.Timestamp.AsTime(),
		MatchType:              &game_status_pb.MatchType,
		HomeTeam:               *NewTeamFromRefereeTeam(game_status_pb.HomeTeam),
		AwayTeam:               *NewTeamFromRefereeTeam(game_status_pb.AwayTeam),
		GameStage:              &game_status_pb.GameStage,
		GameStageTimeLeft:      game_status_pb.GameStageTimeLeft.AsDuration(),
		TotalCommandsIssued:    game_status_pb.TotalCommandsIssued,
		CommandIssuedTimestamp: game_status_pb.CommandIssuedTimestamp.AsTime(),
		Command:                command,
		NextCommand:            nextCommand,
		GameEvents:             game_status_pb.GameEvents,
		GameEventsProposals:    NewGameEventsProposalsFromRefereeGameEventsProposals(game_status_pb.GameEventsProposals),
	}
}

func (gs *GameStatus) ToProto() *playback.GameStatus {
	proposals := make([]*playback.GameStatus_GameEventsProposal, 0)
	for _, proposal := range gs.GameEventsProposals {
		proposals = append(proposals, proposal.ToProto())
	}

	matchType := common.MatchType_MATCH_TYPE_UNSPECIFIED
	if gs.MatchType != nil {
		matchType = *gs.MatchType
	}

	gameStage := common.GameStage_GAME_STAGE_UNSPECIFIED
	if gs.GameStage != nil {
		gameStage = *gs.GameStage
	}

	return &playback.GameStatus{
		SourceId:               gs.SourceId,
		Description:            &gs.Description,
		Timestamp:              timestamppb.New(gs.Timestamp),
		MatchType:              matchType,
		HomeTeam:               gs.HomeTeam.ToProto(),
		AwayTeam:               gs.AwayTeam.ToProto(),
		GameStage:              gameStage,
		GameStageTimeLeft:      durationpb.New(gs.GameStageTimeLeft),
		TotalCommandsIssued:    gs.TotalCommandsIssued,
		CommandIssuedTimestamp: timestamppb.New(gs.CommandIssuedTimestamp),
		Command:                gs.Command,
		NextCommand:            gs.NextCommand,
		GameEvents:             gs.GameEvents,
		GameEventsProposals:    proposals,
	}
}

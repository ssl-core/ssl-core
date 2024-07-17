package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/player-bff/player-sonson/internal/util"
	"github.com/robocin/ssl-core/protocols/common"
	"github.com/robocin/ssl-core/protocols/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Frame struct {
	StartTime           time.Time     `json:"start_time"`
	CurrentTime         time.Time     `json:"current_time"`
	SerialId            uint64        `json:"serial_id"`
	Fps                 uint32        `json:"fps"`
	Balls               []Ball        `json:"balls"`
	Robots              []Robot       `json:"robots"`
	Field               Field         `json:"field"`
	MatchType           string        `json:"match_type"`
	HomeTeam            Team          `json:"home_team"`
	AwayTeam            Team          `json:"away_team"`
	GameStage           string        `json:"game_stage"`
	GameStageTimeLeft   time.Duration `json:"game_stage_time_left"`
	TotalCommands       uint64        `json:"total_commands"`
	CommandTime         time.Time     `json:"command_time"`
	Command             interface{}   `json:"command"`
	NextCommand         interface{}   `json:"next_command"`
	GameEvents          interface{}   `json:"game_events"`
	GameEventsProposals interface{}   `json:"game_events_proposals"`
}

func NewFrame(sample *playback.Sample) Frame {
	field := NewField(util.SetDefaultIfNil(sample.GetField(), &playback.Field{}))

	balls := make([]Ball, len(sample.GetDetection().GetBalls()))
	for i, ball := range sample.GetDetection().GetBalls() {
		balls[i] = NewBall(util.SetDefaultIfNil(ball, &playback.Detection_Ball{}))
	}

	robots := make([]Robot, len(sample.GetDetection().GetRobots()))
	for i, robot := range sample.GetDetection().GetRobots() {
		robots[i] = NewRobot(util.SetDefaultIfNil(robot, &playback.Detection_Robot{}))
	}

	serialId := util.SetDefaultIfNil(sample.GetDetection().GetSerialId(), 0)
	fps := util.SetDefaultIfNil(sample.GetDetection().GetExpectedFramerate(), 0)
	currentTime := util.SetDefaultIfNil(sample.GetTimestamp(), &timestamppb.Timestamp{}).AsTime()
	startTime := util.SetDefaultIfNil(sample.GetFirstTimestamp(), &timestamppb.Timestamp{}).AsTime()

	matchType := translateMatchType(sample.GetGameStatus().GetMatchType())
	homeTeam := NewTeam(sample.GetGameStatus().GetHomeTeam(), sample.GetGameStatus().GetIsHomeTeamOnPositiveHalf())
	awayTeam := NewTeam(sample.GetGameStatus().GetAwayTeam(), !sample.GetGameStatus().GetIsHomeTeamOnPositiveHalf())
	gameStage := sample.GetGameStatus().GetGameStage().String()
	gameStageTimeLeft := sample.GetGameStatus().GetGameStageTimeLeft().AsDuration()
	totalCommands := sample.GetGameStatus().GetTotalCommandsIssued()
	commandTime := sample.GetGameStatus().GetCommandIssuedTimestamp().AsTime()
	command := sample.GetGameStatus().GetCommand()
	nextCommand := sample.GetGameStatus().GetNextCommand()
	gameEvents := sample.GetGameStatus().GetGameEvents()
	gameEventsProposals := sample.GetGameStatus().GetGameEventsProposals()

	return Frame{
		StartTime:           startTime,
		CurrentTime:         currentTime,
		SerialId:            serialId,
		Fps:                 fps,
		Balls:               balls,
		Robots:              robots,
		Field:               field,
		MatchType:           matchType,
		HomeTeam:            homeTeam,
		AwayTeam:            awayTeam,
		GameStage:           gameStage,
		GameStageTimeLeft:   gameStageTimeLeft,
		TotalCommands:       totalCommands,
		CommandTime:         commandTime,
		Command:             command,
		NextCommand:         nextCommand,
		GameEvents:          gameEvents,
		GameEventsProposals: gameEventsProposals,
	}
}

func (f *Frame) ToJson() ([]byte, error) {
	return json.Marshal(f)
}

func translateMatchType(matchType common.MatchType) string {
	switch matchType {
	case common.MatchType_MATCH_TYPE_FRIENDLY:
		return "friendly"
	case common.MatchType_MATCH_TYPE_GROUP_PHASE:
		return "group_phase"
	case common.MatchType_MATCH_TYPE_ELIMINATION_PHASE:
		return "elimination_phase"
	}

	return "unspecified"
}

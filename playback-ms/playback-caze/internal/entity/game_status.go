package entity

import (
	"encoding/json"
	"time"
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
	GameEvents             []GameEvent          `json:"game_events"`
	GameEventsProposals    []GameEventsProposal `json:"game_events_proposals"`
}

func (gs *GameStatus) ToJson() ([]byte, error) {
	return json.Marshal(gs)
}

package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
)

// TODO(mviniciusteotoni): Model the internal GameEvent struct?
type GameEvent common.GameEvent

type GameEventsProposal struct {
	ProposalId  string      `json:"proposal_id"`
	GameEvents  []GameEvent `json:"game_events"`
	WasAccepted bool        `json:"was_accepted"`
}

func (ge *GameEvent) ToJson() ([]byte, error) {
	return json.Marshal(ge)
}

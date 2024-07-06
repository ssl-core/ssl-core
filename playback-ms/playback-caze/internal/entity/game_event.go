package entity

import (
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/gateway"
)

// TODO(mviniciusteotoni): Model the internal GameEvent struct?
type GameEvent = common.GameEvent

// func (ge *GameEvent) ToJson() ([]byte, error) {
// 	return json.Marshal(ge)
// }

type GameEvents struct {
	Events []*GameEvent `json:"events"`
}

func NewGameEvents(events []*GameEvent) *GameEvents {
	return &GameEvents{
		Events: events,
	}
}

func (ge GameEvents) ToProto() *gateway.GetGameEventsResponse {
	return &gateway.GetGameEventsResponse{
		GameEvents: ge.Events,
	}
}

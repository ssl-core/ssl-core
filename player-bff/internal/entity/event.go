package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/player-bff/pkg/pb/common"
)

type Event struct {
}

func NewEvent(gameEvent *common.GameEvent) Event {
	return Event{}
}

func (c *Event) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

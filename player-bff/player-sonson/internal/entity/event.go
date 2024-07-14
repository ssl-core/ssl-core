package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/protocols/common"
)

type Event struct {
	Type    string
	Data    interface{}
	Sources []string
	Time    time.Time
}

func NewEvent(gameEvent *common.GameEvent) Event {
	reflector := gameEvent.ProtoReflect()
	descriptor := reflector.Descriptor()
	event := reflector.WhichOneof(descriptor.Oneofs().ByName("Event"))

	return Event{
		Type:    event.TextName(),
		Data:    event.Message().Fields(),
		Sources: gameEvent.GetSources(),
		Time:    gameEvent.GetTimestamp().AsTime(),
	}
}

func (c *Event) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

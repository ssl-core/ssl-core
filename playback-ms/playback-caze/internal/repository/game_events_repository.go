package repository

import (
	"github.com/robocin/ssl-core/protocols/common"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type IGameEventsRepository interface {
	AddGameEvents(*timestamppb.Timestamp, []*common.GameEvent) error
	GetGameEventsFrom(*timestamppb.Timestamp) ([]*common.GameEvent, error)
}

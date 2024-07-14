package service

import (
	"github.com/robocin/ssl-core/player-bff/player-sonson/internal/entity"
)

type PlaybackService interface {
	GetConn() interface{}
	Close() error
	ReceiveLiveStream() error
	GetReplayChunk() (entity.Chunk, error)
	GetGameEvents() ([]entity.Event, error)
}

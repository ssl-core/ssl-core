package client

import (
	"github.com/robocin/ssl-core/match-bff/internal/entity"
	"golang.org/x/net/websocket"
)

type Clienter interface {
	GetConn() interface{}
	Close() error
	SubscribeToLiveStream(*websocket.Conn)
	UnsubscribeToLiveStream(*websocket.Conn)
	ReceiveLiveStream() error
	GetVisionChunk() (entity.Chunk, error)
}

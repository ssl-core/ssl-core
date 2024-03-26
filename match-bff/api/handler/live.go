package handler

import (
	"github.com/robocin/ssl-core/match-bff/internal/client"
	"golang.org/x/net/websocket"
)

func ReceiveLiveStreamHandler(client client.Clienter) func(conn *websocket.Conn, data map[string]interface{}) {
	return func(conn *websocket.Conn, data map[string]interface{}) {
		client.SubscribeToLiveStream(conn)
		conn.Write([]byte("Subscribed to live stream!"))
	}
}

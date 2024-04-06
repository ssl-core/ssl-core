package handler

import (
	"github.com/robocin/ssl-core/match-bff/internal/client"
	"golang.org/x/net/websocket"
)

func GetReplayChunkHandler(client client.Clienter) func(conn *websocket.Conn, data map[string]interface{}) {
	return func(conn *websocket.Conn, data map[string]interface{}) {
		chunk, err := client.GetVisionChunk()
		if err != nil {
			conn.Write([]byte("Error: " + err.Error()))
			return
		}

		response, err := chunk.ToJson()
		if err != nil {
			conn.Write([]byte("Error: " + err.Error()))
			return
		}

		conn.Write(response)
	}
}

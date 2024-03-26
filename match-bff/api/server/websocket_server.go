package server

import (
	"encoding/json"
	"io"
	"net/http"

	"github.com/robocin/ssl-core/match-bff/api/handler"
	"github.com/robocin/ssl-core/match-bff/internal/client"
	"golang.org/x/net/websocket"
)

type WebsocketServer struct {
	conns  map[*websocket.Conn]bool
	client client.Clienter
}

func NewWebsocketServer(client client.Clienter) *WebsocketServer {
	return &WebsocketServer{
		conns:  make(map[*websocket.Conn]bool),
		client: client,
	}
}

func (ws *WebsocketServer) GetHandler() http.Handler {
	return websocket.Handler(ws.handleConnection)
}

func (ws *WebsocketServer) Broadcast(message []byte) {
	for conn := range ws.conns {
		go func(conn *websocket.Conn) {
			if _, err := conn.Write(message); err != nil {
				conn.Close()
			}
		}(conn)
	}
}

func (ws *WebsocketServer) handleConnection(conn *websocket.Conn) {
	ws.conns[conn] = true
	defer delete(ws.conns, conn)

	ws.listenConnection(conn)
}

func (ws *WebsocketServer) listenConnection(conn *websocket.Conn) {
	buf := make([]byte, 1024)
	for {
		size, err := conn.Read(buf)

		if err != nil {
			if err == io.EOF {
				break
			}

			continue
		}

		message := map[string]interface{}{}
		if err = json.Unmarshal(buf[:size], &message); err != nil {
			conn.Write([]byte("Error: " + err.Error()))
			continue
		}

		event := message["event"].(string)
		data := message["data"].(map[string]interface{})
		ws.handleMessage(conn, event, data)
	}
}

func (ws *WebsocketServer) handleMessage(conn *websocket.Conn, event string, data map[string]interface{}) {
	switch event {
	case "receive-live-stream":
		handler.ReceiveLiveStreamHandler(ws.client)(conn, data)
	case "get-chunk":
		handler.GetChunkHandler(ws.client)(conn, data)
	case "ping":
		conn.Write([]byte("pong"))
	case "close":
		conn.Close()
	default:
		conn.Write([]byte("echo: " + string(event)))
	}
}

package server

import (
	"net/http"

	"golang.org/x/net/websocket"
)

type WebsocketServer struct {
	conns map[*websocket.Conn]bool
}

func NewWebsocketServer() *WebsocketServer {
	return &WebsocketServer{
		conns: make(map[*websocket.Conn]bool),
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
			if err.Error() == "EOF" {
				break
			}

			continue
		}

		message := buf[:size]
		conn.Write([]byte("echo: " + string(message)))
	}
}

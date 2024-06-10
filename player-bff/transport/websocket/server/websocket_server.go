package server

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
	"github.com/robocin/ssl-core/player-bff/transport/grpc/client"
	"github.com/robocin/ssl-core/player-bff/transport/websocket/connection"
)

type WebsocketServer struct {
	upgrader *websocket.Upgrader
	router   *http.ServeMux
	client   *client.GrpcClient
}

func NewWebsocketServer(router *http.ServeMux, client *client.GrpcClient) *WebsocketServer {
	upgrader := &websocket.Upgrader{
		ReadBufferSize:  1024,
		WriteBufferSize: 1024,
	}

	server := &WebsocketServer{
		upgrader: upgrader,
		router:   router,
		client:   client,
	}

	server.registerHandler()

	return server
}

func (ws *WebsocketServer) registerHandler() {
	ws.router.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		conn, err := ws.upgrader.Upgrade(w, r, nil)

		if err != nil {
			log.Println(err)
			return
		}

		websocketConnection := connection.NewWebsocketConnection(conn, ws.client)
		websocketConnection.Listen()
	})
}

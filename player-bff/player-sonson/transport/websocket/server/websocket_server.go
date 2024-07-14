package server

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
	"github.com/robocin/ssl-core/player-bff/player-sonson/internal/application"
	"github.com/robocin/ssl-core/player-bff/player-sonson/transport/grpc/client"
	"github.com/robocin/ssl-core/player-bff/player-sonson/transport/websocket/connection"
)

type WebsocketServer struct {
	upgrader    *websocket.Upgrader
	router      *http.ServeMux
	client      *client.GrpcClient
	sharedProxy *application.ConnectionProxy
}

func NewWebsocketServer(router *http.ServeMux, client *client.GrpcClient, sharedProxy *application.ConnectionProxy) *WebsocketServer {
	upgrader := &websocket.Upgrader{
		ReadBufferSize:  8192,
		WriteBufferSize: 8192,
		CheckOrigin:     func(r *http.Request) bool { return true },
	}

	return &WebsocketServer{
		upgrader:    upgrader,
		router:      router,
		client:      client,
		sharedProxy: sharedProxy,
	}
}

func (ws *WebsocketServer) Register() {
	ws.router.HandleFunc("/ws", func(w http.ResponseWriter, r *http.Request) {
		conn, err := ws.upgrader.Upgrade(w, r, nil)

		if err != nil {
			log.Println(err)
			return
		}

		websocketConnection := connection.NewWebsocketConnection(conn, ws.client, ws.sharedProxy)
		websocketConnection.Listen()
	})
}

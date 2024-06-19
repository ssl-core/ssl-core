package connection

import (
	"github.com/gorilla/websocket"
	"github.com/robocin/ssl-core/player-bff/internal/application"
	"github.com/robocin/ssl-core/player-bff/transport/grpc/client"
)

type WebsocketConnectionState uint8

const (
	Available WebsocketConnectionState = iota
	Live
)

type WebsocketConnection struct {
	state       WebsocketConnectionState
	conn        *websocket.Conn
	client      *client.GrpcClient
	sharedProxy *application.ConnectionProxy
}

func NewWebsocketConnection(conn *websocket.Conn, client *client.GrpcClient, sharedProxy *application.ConnectionProxy) *WebsocketConnection {
	return &WebsocketConnection{
		state:       Available,
		conn:        conn,
		client:      client,
		sharedProxy: sharedProxy,
	}
}

func (wc *WebsocketConnection) Listen() {
	defer wc.conn.Close()

	for {
		var message map[string]interface{}
		err := wc.conn.ReadJSON(&message)

		if err != nil {
			wc.conn.WriteMessage(websocket.TextMessage, []byte("Error: "+err.Error()))
			continue
		}

		event, _ := message["event"].(string)
		data, _ := message["data"].(map[string]interface{})
		go wc.handleEvent(event, data)
	}
}

func (wc *WebsocketConnection) OnNotify(event application.ConnectionProxyEvent) {
	switch event.Type {
	case "sample":
		wc.conn.WriteJSON(event.Payload)
	}
}

func (wc *WebsocketConnection) handleEvent(event string, data map[string]interface{}) {
	switch event {
	case "receive-live-stream":
		wc.changeState(Live)
		wc.sharedProxy.AddListener(wc)
	case "get-chunk":
		wc.changeState(Available)
		timestamp := data["timestamp"].(string)
		wc.client.GetReplayChunk(timestamp)
	case "ping":
		wc.conn.WriteMessage(websocket.TextMessage, []byte("pong"))
	case "close":
		wc.conn.Close()
	default:
		wc.conn.WriteMessage(websocket.TextMessage, []byte("echo: "+string(event)))
	}
}

func (wc *WebsocketConnection) changeState(newState WebsocketConnectionState) {
	if wc.state == newState {
		return
	}

	if wc.state == Live {
		wc.sharedProxy.RemoveListener(wc)
	}

	wc.state = newState
}

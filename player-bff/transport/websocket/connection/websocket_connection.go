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
	Replay
)

type WebsocketConnection struct {
	state  WebsocketConnectionState
	conn   *websocket.Conn
	client *client.GrpcClienter
}

func NewWebsocketConnection(conn *websocket.Conn, client *client.GrpcClient) *WebsocketConnection {
	return &WebsocketConnection{
		state:  Available,
		conn:   conn,
		client: client,
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
	case "frame":
		wc.conn.WriteJSON(event.Payload)
	}
}

func (wc *WebsocketConnection) handleEvent(event string, data map[string]interface{}) {
	switch event {
	case "receive-live-stream":
		if wc.state == Replay {
			wc.conn.WriteMessage(websocket.TextMessage, []byte("Error: Cannot receive live stream while replaying"))
			return
		}

		if wc.state == Live {
			wc.conn.WriteMessage(websocket.TextMessage, []byte("Error: Already receiving live stream"))
			return
		}

		wc.state = Live

		proxy := application.NewConnectionProxy()
		proxy.AddListener(wc)
		wc.client.ReceiveLiveStream(proxy)
	case "ping":
		wc.conn.WriteMessage(websocket.TextMessage, []byte("pong"))
	case "close":
		wc.conn.Close()
	default:
		wc.conn.WriteMessage(websocket.TextMessage, []byte("echo: "+string(event)))
	}
}

package receiver

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/network"
)

type MessageReceiver struct {
	handlers []*SocketHandler
}

type ISocketReceiver interface {
	Receive() network.ZmqMultipartDatagram
}

type SocketHandler struct {
	socket   ISocketReceiver
	callback func(network.ZmqMultipartDatagram)
}

func (sh *SocketHandler) WhenReceive() {
	datagram := sh.socket.Receive()

	if !datagram.IsEmpty() {
		sh.callback(datagram)
	}
}

func NewSocketHandler(socket ISocketReceiver, callback func(network.ZmqMultipartDatagram)) *SocketHandler {
	return &SocketHandler{
		socket:   socket,
		callback: callback,
	}
}

func NewMessageReceiver(handlers []*SocketHandler) *MessageReceiver {
	return &MessageReceiver{
		handlers: handlers,
	}
}

func (mr *MessageReceiver) Start(wg *sync.WaitGroup) {
	for _, handler := range mr.handlers {
		wg.Add(1)

		go func() {
			defer wg.Done()

			for {
				handler.WhenReceive()
			}
		}()
	}
}

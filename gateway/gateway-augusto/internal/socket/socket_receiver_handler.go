package socket

import (
	"github.com/robocin/ssl-core/common/golang/network"
)

type ISocketReceiver interface {
	Receive() network.ZmqMultipartDatagram
}

type SocketReceiverHandler struct {
	socket   ISocketReceiver
	callback func(network.ZmqMultipartDatagram)
}

func (sh *SocketReceiverHandler) OnReceive() network.ZmqMultipartDatagram {
	datagram := sh.socket.Receive()

	if !datagram.IsEmpty() {
		sh.callback(datagram)
	}

	return datagram
}

func NewSocketReceiverHandler(socket ISocketReceiver, callback func(network.ZmqMultipartDatagram)) *SocketReceiverHandler {
	if callback == nil {
		callback = func(datagram network.ZmqMultipartDatagram) {
			// Default behavior: no-op
		}
	}

	return &SocketReceiverHandler{
		socket:   socket,
		callback: callback,
	}
}

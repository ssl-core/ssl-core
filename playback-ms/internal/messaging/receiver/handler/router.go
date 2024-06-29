package handler

import "github.com/robocin/ssl-core/playback-ms/network"

type RouterHandler struct {
	socket  *network.ZmqRouterSocket
	channel *chan network.ZmqMultipartDatagram
}

func NewRouterHandler(socket *network.ZmqRouterSocket, channel *chan network.ZmqMultipartDatagram) *RouterHandler {
	return &RouterHandler{socket, channel}
}

func (r *RouterHandler) receive() network.ZmqMultipartDatagram {
	return r.socket.Receive()
}

func (r *RouterHandler) close() {
	r.socket.Close()
}

func (r *RouterHandler) Handle() {
	defer r.close()
	for {
		datagram := r.receive()
		*r.channel <- datagram
	}
}

package handler

import "github.com/robocin/ssl-core/playback-ms/network"

type Handler interface {
	receive() network.ZmqMultipartDatagram
	close()
	Handle()
}

type BaseHandler struct {
	socket  socket
	channel *chan network.ZmqMultipartDatagram
}

type socket interface {
	Receive() network.ZmqMultipartDatagram
	Close()
}

func newBaseHandler(socket socket, channel *chan network.ZmqMultipartDatagram) *BaseHandler {
	return &BaseHandler{socket: socket, channel: channel}
}

func (b *BaseHandler) Handle() {
	defer b.close()
	for {
		datagram := b.receive()
		*b.channel <- datagram
	}
}

func (b *BaseHandler) close() {
	b.socket.Close()
}

func (b *BaseHandler) receive() network.ZmqMultipartDatagram {
	return b.socket.Receive()
}

type RouterHandler struct {
	*BaseHandler
}

func NewRouterHandler(socket *network.ZmqRouterSocket, channel *chan network.ZmqMultipartDatagram) *RouterHandler {
	return &RouterHandler{newBaseHandler(socket, channel)}
}

type SubscriberHandler struct {
	*BaseHandler
}

func NewSubscriberHandler(socket *network.ZmqSubscriberSocket, channel *chan network.ZmqMultipartDatagram) *SubscriberHandler {
	return &SubscriberHandler{newBaseHandler(socket, channel)}
}

package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/network"
)

type Handler interface {
	receive() network.ZmqMultipartDatagram
	close()
	Handle()
}

type BaseHandler struct {
	id      string
	socket  socket
	channel *chan network.ZmqMultipartDatagram
}

type socket interface {
	Receive() network.ZmqMultipartDatagram
	Close()
}

func newBaseHandler(id string, socket socket, channel *chan network.ZmqMultipartDatagram) *BaseHandler {
	return &BaseHandler{id: id, socket: socket, channel: channel}
}

func (b *BaseHandler) Handle() {
	defer b.close()

	fmt.Printf("Starting handler for %s...\n", b.id)
	for {
		datagram := b.receive()
		fmt.Printf("Received: %v\n", datagram)
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

func NewRouterHandler(id string, socket *network.ZmqRouterSocket, channel *chan network.ZmqMultipartDatagram) *RouterHandler {
	return &RouterHandler{newBaseHandler(id, socket, channel)}
}

type SubscriberHandler struct {
	*BaseHandler
}

func NewSubscriberHandler(id string, socket *network.ZmqSubscriberSocket, channel *chan network.ZmqMultipartDatagram) *SubscriberHandler {
	return &SubscriberHandler{newBaseHandler(id, socket, channel)}
}

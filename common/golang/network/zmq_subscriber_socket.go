package network

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqSubscriberSocket struct {
	Socket *goczmq.Sock
}

func NewZmqSubscriberSocket(address string, topics string) *ZmqSubscriberSocket {
	socket, err := goczmq.NewSub(address, topics)

	if err != nil {
		panic(err)
	}

	return &ZmqSubscriberSocket{
		Socket: socket,
	}
}

func (socket *ZmqSubscriberSocket) Receive() ZmqMultipartDatagram {
	bytes, err := socket.Socket.RecvMessage()

	if err != nil {
		fmt.Println("failed to receive message:", err)
		return ZmqMultipartDatagram{}
	}

	return NewZmqMultipartDatagram(bytes[0], bytes[1])
}

func (socket *ZmqSubscriberSocket) Close() {
	socket.Socket.Destroy()
}

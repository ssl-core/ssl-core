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

func (sock *ZmqSubscriberSocket) Receive() ZmqDatagram {
	bytes, err := sock.Socket.RecvMessage()
	fmt.Println("Topic", string(bytes[0]), "Message", string(bytes[1]))

	if err != nil {
		fmt.Println("failed to receive message", err)
		return *NewZmqDatagram("", nil)
	}

	// TODO: treament for topics
	return *NewZmqDatagram(string(bytes[0]), bytes[1])
}

func (sock *ZmqSubscriberSocket) Close() {
	sock.Socket.Destroy()
}

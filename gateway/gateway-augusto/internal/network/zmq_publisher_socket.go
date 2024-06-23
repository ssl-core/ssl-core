package network

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqPublisherSocket struct {
	address string
	socket  *goczmq.Sock
}

func NewZmqPublisherSocket(address string) *ZmqPublisherSocket {
	socket, err := goczmq.NewPub(address)

	if err != nil {
		panic(err)
	}

	return &ZmqPublisherSocket{
		address: address,
		socket:  socket,
	}
}

func (pub *ZmqPublisherSocket) Bind(address string) {
	pub.socket.Bind(pub.address)
}

func (pub *ZmqPublisherSocket) Send(datagram ZmqDatagram) error {
	err := pub.socket.SendFrame([]byte(datagram.topic), goczmq.FlagMore)
	if err != nil {
		return fmt.Errorf("failed to send topic: %w", err)
	}

	err = pub.socket.SendFrame(datagram.message, goczmq.FlagNone)
	if err != nil {
		return fmt.Errorf("failed to send message: %w", err)
	}

	return nil
}

func (pub *ZmqPublisherSocket) Close() {
	pub.socket.Destroy()
}

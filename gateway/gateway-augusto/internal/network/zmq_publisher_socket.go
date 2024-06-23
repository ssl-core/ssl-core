package network

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqPublisherSocket struct {
	Socket *goczmq.Sock
}

func NewZmqPublisherSocket(address string) *ZmqPublisherSocket {
	socket, err := goczmq.NewPub(address)

	if err != nil {
		panic(err)
	}

	return &ZmqPublisherSocket{
		Socket: socket,
	}
}

func (sock *ZmqPublisherSocket) Send(datagram ZmqDatagram) error {
	err := sock.Socket.SendFrame([]byte(datagram.topic), goczmq.FlagMore)
	if err != nil {
		return fmt.Errorf("failed to send topic: %w", err)
	}

	err = sock.Socket.SendFrame(datagram.message, goczmq.FlagNone)
	if err != nil {
		return fmt.Errorf("failed to send message: %w", err)
	}

	return nil
}

func (sock *ZmqPublisherSocket) Close() {
	sock.Socket.Destroy()
}

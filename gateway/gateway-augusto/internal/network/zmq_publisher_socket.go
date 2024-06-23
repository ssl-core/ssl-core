package network

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqPublisherSocket struct {
	socket *goczmq.Sock
}

func NewZmqPublisherSocket(address string) *ZmqPublisherSocket {
	socket, err := goczmq.NewPub(address)

	if err != nil {
		panic(err)
	}

	return &ZmqPublisherSocket{
		socket: socket,
	}
}

func (sock *ZmqPublisherSocket) Send(datagram ZmqDatagram) error {
	err := sock.socket.SendFrame([]byte(datagram.Topic), goczmq.FlagMore)
	if err != nil {
		return fmt.Errorf("failed to send topic: %w", err)
	}

	err = sock.socket.SendFrame(datagram.Message, goczmq.FlagNone)
	if err != nil {
		return fmt.Errorf("failed to send message: %w", err)
	}

	return nil
}

func (sock *ZmqPublisherSocket) Close() {
	sock.socket.Destroy()
}

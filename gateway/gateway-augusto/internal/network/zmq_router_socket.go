package network

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqRouterSocket struct {
	socket *goczmq.Sock
}

func NewZmqRouterSocket(address string) *ZmqRouterSocket {
	socket, err := goczmq.NewRouter(address)

	if err != nil {
		panic(err)
	}

	return &ZmqRouterSocket{
		socket: socket,
	}
}

func (sock *ZmqRouterSocket) Send(datagram ZmqMultipartDatagram) error {
	return sock.socket.SendMessage([][]byte{datagram.Identifier, datagram.Message})
}

func (sock *ZmqRouterSocket) Receive() ZmqMultipartDatagram {
	message, err := sock.socket.RecvMessage()

	if err != nil {
		fmt.Errorf("failed to receive message")
		return *NewZmqMultipartDatagram(nil, nil)
	}

	return *NewZmqMultipartDatagram(message[0], message[1])
}

func (sock *ZmqRouterSocket) Test() {
	message, err := sock.socket.RecvMessage()

	if err != nil {
		fmt.Errorf("failed to receive message")
		return
	}

	fmt.Println("Response to", message[0], string(message[1]))
	sock.socket.SendMessage([][]byte{message[0], []byte("response")})
}

func (sock *ZmqRouterSocket) Close() {
	sock.socket.Destroy()
}

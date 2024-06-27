package transport

import (
	"fmt"

	"github.com/zeromq/goczmq"
)

type ZmqDealerSocket struct {
	socket *goczmq.Sock
}

func NewZmqDealerSocket(address string) *ZmqDealerSocket {
	socket, err := goczmq.NewDealer(address)

	if err != nil {
		panic(err)
	}

	return &ZmqDealerSocket{
		socket: socket,
	}
}

func (sock *ZmqDealerSocket) Send(datagram ZmqDatagram) {
	sock.socket.SendMessage([][]byte{datagram.Message})
}

func (sock *ZmqDealerSocket) Receive() ZmqDatagram {
	message, err := sock.socket.RecvMessage()

	if err != nil {
		fmt.Errorf("failed to receive message")
		return *NewZmqDatagram(nil)
	}

	return *NewZmqDatagram(message[0])
}

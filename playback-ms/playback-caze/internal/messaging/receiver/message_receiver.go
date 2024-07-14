package receiver

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/zeromq/goczmq"
)

const (
	pollerTimeout = 10 /*ms ~= 100hz*/
)

type MessageReceiver struct {
	subscribers []*network.ZmqSubscriberSocket
	poller      *goczmq.Poller
}

func NewMessageReceiver(sockets []*network.ZmqSubscriberSocket) *MessageReceiver {
	poller, err := goczmq.NewPoller()

	if err != nil {
		panic("error creating zmq poller")
	}

	for i := range sockets {
		poller.Add(sockets[i].Socket)
	}

	return &MessageReceiver{
		subscribers: sockets,
		poller:      poller,
	}
}

func (mr *MessageReceiver) Start(datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram], wg *sync.WaitGroup) {
	wg.Add(1)

	go func() {
		defer wg.Done()

		for {
			socket := mr.poller.Wait(pollerTimeout)
			if socket == nil {
				continue
			}

			for {
				datagram := receiveFrom(socket)
				if datagram.IsEmpty() {
					break
				}

				datagrams.Enqueue(datagram)
			}
		}
	}()
}

func receiveFrom(socket *goczmq.Sock) network.ZmqMultipartDatagram {
	bytes, err := socket.RecvMessageNoWait()

	if len(bytes) == 0 || err != nil {
		return network.ZmqMultipartDatagram{}
	}

	return network.NewZmqMultipartDatagram(bytes[0], bytes[1])
}

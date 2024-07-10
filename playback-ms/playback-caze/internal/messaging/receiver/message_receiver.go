package receiver

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type MessageReceiver struct {
	subscribers []*network.ZmqSubscriberSocket
}

func NewMessageReceiver(sockets []*network.ZmqSubscriberSocket) *MessageReceiver {
	return &MessageReceiver{
		subscribers: sockets,
	}
}

func (mr *MessageReceiver) Start(datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram], wg *sync.WaitGroup) {
	for _, sub := range mr.subscribers {
		wg.Add(1)

		go func() {
			defer wg.Done()

			for {
				datagram := sub.Receive()
				if datagram.IsEmpty() {
					continue
				}
				datagrams.Enqueue(datagram)
			}
		}()
	}
}

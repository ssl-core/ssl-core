package receiver

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type MessageReceiver struct {
	subscribers []*network.ZmqSubscriberSocket
	routers     []*network.ZmqRouterSocket
}

func NewMessageReceiver(subscribers []*network.ZmqSubscriberSocket, routers []*network.ZmqRouterSocket) *MessageReceiver {
	return &MessageReceiver{
		subscribers: subscribers,
		routers:     routers,
	}
}

func (mr *MessageReceiver) Start(subscribersDatagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram], routersDatagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram], wg *sync.WaitGroup) {
	for _, sub := range mr.subscribers {
		wg.Add(1)

		go func() {
			defer wg.Done()

			for {
				datagram := sub.Receive()
				if datagram.IsEmpty() {
					continue
				}
				subscribersDatagrams.Enqueue(datagram)
			}
		}()
	}

	for _, router := range mr.routers {
		wg.Add(1)

		go func() {
			defer wg.Done()

			for {
				datagram := router.Receive()
				if datagram.IsEmpty() {
					continue
				}
				routersDatagrams.Enqueue(datagram)
			}
		}()
	}
}

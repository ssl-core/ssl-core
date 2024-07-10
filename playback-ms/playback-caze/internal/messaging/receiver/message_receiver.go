package receiver

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/zeromq/goczmq"
)

type MessageReceiver struct {
	channel     chan<- network.ZmqMultipartDatagram
	subscribers []*network.ZmqSubscriberSocket
	poller      *goczmq.Poller
}

func NewMessageReceiver(channel chan<- network.ZmqMultipartDatagram, sockets []*network.ZmqSubscriberSocket) *MessageReceiver {
	poller, err := goczmq.NewPoller()

	if err != nil {
		panic("poller not created")
	}

	for i := range sockets {
		poller.Add(sockets[i].Socket)
	}

	return &MessageReceiver{
		channel:     channel,
		subscribers: sockets,
		poller:      poller,
	}
}

func (mr *MessageReceiver) subscriberHandler(wg *sync.WaitGroup, id int) {
	defer wg.Done()

	count := 0
	for {
		datagram := mr.subscribers[id].Receive()
		if datagram.IsEmpty() {
			continue
		}
		count++
		fmt.Println("Receiving ", id, count)
		// mr.channel <- datagram
	}
}

func (mr *MessageReceiver) Start(wg *sync.WaitGroup) {
	// wg.Add(2)
	for id := range mr.subscribers {
		wg.Add(1)
		go mr.subscriberHandler(wg, id)
	}

	// fmt.Println("starting message receiver")
	// go mr.subscriberSocketsHandler(wg)
	// go routerSocketHandler(wg)
}

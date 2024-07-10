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

func (mr *MessageReceiver) subscriberSocketsHandler(wg *sync.WaitGroup) {
	defer wg.Done()

	for {
		msg := mr.subscribers[0].Receive()
		if !msg.IsEmpty() {
			fmt.Println("Received 1")
			continue
		}

		msg2 := mr.subscribers[1].Receive()
		if !msg2.IsEmpty() {
			fmt.Println("Received 2")
			continue
		}

		fmt.Println("empty")
		// socket := mr.poller.Wait(1000)
		// if socket == nil {
		// 	fmt.Println("socket nil")
		// 	continue
		// }
		// fmt.Println("adding message in channel")
		// mr.channel <- mr.subscriberReceive(*socket)
	}
}

func (mr *MessageReceiver) subscriberReceive(subscriber goczmq.Sock) network.ZmqMultipartDatagram {
	bytes, err := subscriber.RecvMessage()

	if err != nil {
		fmt.Println("failed to receive message", err)
		return *network.NewZmqMultipartDatagram(nil, nil)
	}

	return *network.NewZmqMultipartDatagram(bytes[0], bytes[1])
}

func (mr *MessageReceiver) Start(wg *sync.WaitGroup) {
	wg.Add(1)
	fmt.Println("starting message receiver")
	go mr.subscriberSocketsHandler(wg)
	// go routerSocketHandler(wg)
}

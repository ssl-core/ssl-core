package stub

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type SubscriberStub struct {
	subscriber *network.ZmqSubscriberSocket
}

func NewSubscriberStub() *SubscriberStub {
	return &SubscriberStub{
		subscriber: network.NewZmqSubscriberSocket(service_discovery.GetInstance().GetPerceptionAddress(), service_discovery.GetInstance().GetDetectionWrapperTopic()),
	}
}

func (ss *SubscriberStub) Run(wg *sync.WaitGroup) {
	defer wg.Done()

	fmt.Printf("PerceptionStub running...\n")
	for {
		datagram := ss.subscriber.Receive()
		fmt.Printf("Received: %v\n", datagram)
	}
}

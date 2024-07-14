package stub

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/service_discovery"
)

type SubscriberStub struct {
	subscriber *network.ZmqSubscriberSocket
}

func NewSubscriberStub() *SubscriberStub {
	return &SubscriberStub{
		subscriber: network.NewZmqSubscriberSocket(service_discovery.PerceptionAddress, service_discovery.PerceptionDetectionWrapperTopic),
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

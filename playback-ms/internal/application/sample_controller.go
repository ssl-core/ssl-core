package application

import (
	"fmt"
	"log"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

const (
	liveFrequencyHZ = 60
)

type SampleController struct {
	latest_sample   entity.Sample
	datagrams       []network.ZmqMultipartDatagram
	datagramChannel chan network.ZmqMultipartDatagram
	mu              sync.Mutex
	wg              *sync.WaitGroup
	sender          messaging.MessageSender
	ticker          *time.Ticker
}

func NewSampleController(sender messaging.MessageSender, datagramChannel chan network.ZmqMultipartDatagram) *SampleController {
	wg := sync.WaitGroup{}
	wg.Add(2)

	return &SampleController{
		latest_sample:   entity.Sample{},
		datagrams:       make([]network.ZmqMultipartDatagram, 0),
		datagramChannel: datagramChannel,
		mu:              sync.Mutex{},
		wg:              &wg,
		sender:          sender,
		ticker:          time.NewTicker(time.Second / liveFrequencyHZ),
	}
}

func (sc *SampleController) updateLatestSample() {
	defer sc.wg.Done()

	for datagram := range sc.datagramChannel {
		sc.mu.Lock()
		defer sc.mu.Unlock()
		sc.datagrams = append(sc.datagrams, datagram)
		sc.latest_sample.UpdateFromDatagram(datagram)
	}
}

// TODO(matheusvtna): Change the name of this function...
func (sc *SampleController) sendLatestSample() {
	defer sc.ticker.Stop()
	defer sc.wg.Done()

	// currentTime := time.Now()
	for range sc.ticker.C {
		// fmt.Printf("Sending latest sample... (deltaT(ms) == %v)\n", time.Since(currentTime).Milliseconds())
		// currentTime = time.Now()
		sc.mu.Lock()
		if len(sc.datagrams) > 0 {
			go saveToDatabase(sc.datagrams)
			go sc.sender.SendSample(sc.latest_sample)
			sc.datagrams = nil
		}
		sc.mu.Unlock()
	}
}

func makeEntityFromDatagram(datagram network.ZmqMultipartDatagram) (interface{}, error) {
	switch string(datagram.Identifier) {
	case service_discovery.GetInstance().GetDetectionWrapperTopic():
		return entity.NewDetectionFromString(string(datagram.Message))
	default:
		log.Fatalf("Unknown datagram identifier: %v", datagram.Identifier)
		return nil, fmt.Errorf("Unknown datagram identifier: %v", datagram.Identifier)
	}

}

func saveToDatabase(datagrams []network.ZmqMultipartDatagram) {
	client := redis.NewRedisClient()
	defer client.Close()

	for _, datagram := range datagrams {
		entity, err := makeEntityFromDatagram(datagram)
		if err != nil {
			log.Fatalf("Failed to create detection from datagram: %v", err)
		}
		client.Set(string(datagram.Identifier), entity)
	}
}

func (sc *SampleController) Run() {
	defer fmt.Printf("Finishing SampleController\n")

	fmt.Printf("Running SampleController...\n")
	go sc.updateLatestSample()
	go sc.sendLatestSample()
	sc.wg.Wait()
}

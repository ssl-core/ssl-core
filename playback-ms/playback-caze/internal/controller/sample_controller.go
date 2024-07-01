package controller

import (
	"fmt"
	"log"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
)

const (
	liveFrequencyHZ = 120
)

type SampleController struct {
	channel    *chan network.ZmqMultipartDatagram
	wg         *sync.WaitGroup
	sender     *sender.MessageSender
	liveTicker *time.Ticker
	db_client  *redis.RedisClient
}

func NewSampleController(sender *sender.MessageSender, channel *chan network.ZmqMultipartDatagram) *SampleController {
	wg := sync.WaitGroup{}
	wg.Add(3)

	return &SampleController{
		channel:    channel,
		wg:         &wg,
		sender:     sender,
		liveTicker: time.NewTicker(time.Second / liveFrequencyHZ),
		db_client:  redis.NewRedisClient(ChunkStream),
	}
}

func (sc *SampleController) updateLatestSample() {
	for datagram := range *sc.channel {
		world.GetInstance().UpdateFromDatagram(&datagram)
	}
}

func (sc *SampleController) sendLatestSample() {
	defer sc.liveTicker.Stop()
	defer sc.wg.Done()

	for range sc.liveTicker.C {
		sample, err := world.GetInstance().GetLatestSample()
		if err != nil {
			log.Printf("Error getting sample on sendLatestSample: %v\n", err)
			continue
		}

		go sc.sender.SendSample(*sample)
	}
}

func (sc *SampleController) saveSamples() {
	defer sc.wg.Done()

	// TODO(urfs): Does not access the UnsavedSamplesChannel directly from the world instance.
	for sample := range world.GetInstance().UnsavedSamplesChannel {
		sc.db_client.Set(sample.Timestamp, sample)
	}
}

func (sc *SampleController) Run(wg *sync.WaitGroup) {
	defer fmt.Printf("Finishing SampleController\n")
	defer wg.Done()

	fmt.Printf("Running SampleController...\n")
	go sc.updateLatestSample()
	go sc.saveSamples()
	go sc.sendLatestSample()
	sc.wg.Wait()
}

package controller

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
)

const (
	liveFrequencyHZ = 120
)

type SampleController struct {
	datagrams  chan network.ZmqMultipartDatagram
	samples    chan entity.Sample
	wg         *sync.WaitGroup
	sender     *sender.MessageSender
	liveTicker *time.Ticker
	db_client  *redis.RedisClient
}

func NewSampleController(
	sender *sender.MessageSender,
	datagrams chan network.ZmqMultipartDatagram,
	samples chan entity.Sample,
) *SampleController {
	wg := sync.WaitGroup{}
	wg.Add(3)

	return &SampleController{
		datagrams:  datagrams,
		samples:    samples,
		wg:         &wg,
		sender:     sender,
		liveTicker: time.NewTicker(time.Second / liveFrequencyHZ),
		db_client:  redis.NewRedisClient(ChunkStream),
	}
}

func (sc *SampleController) updateLatestSample() {
	for datagram := range sc.datagrams {
		world.GetInstance().UpdateFromDatagram(&datagram)
	}
}

func (sc *SampleController) sendLatestSample() {
	defer sc.liveTicker.Stop()
	defer sc.wg.Done()

	for range sc.liveTicker.C {
		sample, err := world.GetInstance().GetLatestSample()
		if err != nil {
			continue
		}

		go sc.sender.SendSample(*sample)
	}
}

func (sc *SampleController) saveSamples() {
	defer sc.wg.Done()

	for sample := range sc.samples {
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

package controller

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/repository"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/proto"
)

type LiveController struct {
	sender           *sender.MessageSender
	datagrams        *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram]
	handler          *handler.LiveHandler
	samples          *concurrency.ConcurrentQueue[*playback.Sample]
	sampleRepository repository.ISampleRepository
}

func NewLiveController(
	sender *sender.MessageSender,
	datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram],
	handler *handler.LiveHandler,
	sampleRepository repository.ISampleRepository,
) *LiveController {
	return &LiveController{
		datagrams:        datagrams,
		sender:           sender,
		handler:          handler,
		samples:          concurrency.NewQueue[*playback.Sample](),
		sampleRepository: sampleRepository,
	}
}

func (lc *LiveController) Run(wg *sync.WaitGroup) {
	defer wg.Done()

	wg.Add(1)
	go func() {
		defer wg.Done()

		for {
			lc.sampleRepository.AddSamples(lc.samples.DequeueAllWaitLen(120))
		}
	}()

	for {
		for _, datagram := range lc.datagrams.DequeueAll() {
			sample, err := lc.handler.Process(&datagram)
			if err == nil {
				lc.sender.SendSample(sample)
				lc.samples.Enqueue(proto.Clone(sample).(*playback.Sample))
			}
		}
	}
}

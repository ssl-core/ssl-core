package controller

import (
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type LiveController struct {
	sender    *sender.MessageSender
	datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram]
	handler   *handler.LiveHandler
}

func NewLiveController(
	sender *sender.MessageSender,
	datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram],
	handler *handler.LiveHandler,
) *LiveController {
	return &LiveController{
		datagrams: datagrams,
		sender:    sender,
		handler:   handler,
	}
}

func (lc *LiveController) Run(wg *sync.WaitGroup) {
	defer wg.Done()

	for {
		for _, datagram := range lc.datagrams.DequeueAll() {
			sample, err := lc.handler.Process(&datagram)

			if err == nil {
				lc.sender.SendSample(sample)
			}
		}
	}
}

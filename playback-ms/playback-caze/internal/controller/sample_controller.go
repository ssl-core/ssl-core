package controller

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type LiveController struct {
	datagrams <-chan network.ZmqMultipartDatagram
	sender    *sender.MessageSender
	handler   *handler.LiveHandler
}

func NewLiveController(
	sender *sender.MessageSender,
	datagrams <-chan network.ZmqMultipartDatagram,
	handler *handler.LiveHandler,
) *LiveController {
	return &LiveController{
		datagrams: datagrams,
		sender:    sender,
		handler:   handler,
	}
}

func (sc *LiveController) Run(wg *sync.WaitGroup) {
	defer fmt.Printf("Finishing LiveController\n")
	defer wg.Done()
	for {
		for datagram := range sc.datagrams {
			fmt.Printf(string(datagram.Identifier))
			sample, err := sc.handler.Process(&datagram)
			if err == nil {
				sc.sender.SendSample(sample)
			}
		}
	}
}

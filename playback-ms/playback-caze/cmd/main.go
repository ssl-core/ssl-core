package main

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/internal/controller"
	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func runPlayback(wg *sync.WaitGroup) {
	// TODO: move to a controller.
	// receiver:

	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.PerceptionAddress,
		service_discovery.PerceptionDetectionWrapperTopic,
	)

	refereeSocket := network.NewZmqSubscriberSocket(
		service_discovery.RefereeAddress,
		service_discovery.RefereeGameStatusTopic,
	)

	message_receiver := receiver.NewMessageReceiver(
		[]*network.ZmqSubscriberSocket{
			perceptionSocket,
			refereeSocket,
		},
	)

	datagrams := concurrency.NewQueue[network.ZmqMultipartDatagram]()

	// creating goroutines internally but 'Start' does not need to be called by a goroutine.
	message_receiver.Start(datagrams, wg)

	// sender:
	messageSender := sender.NewMessageSender(service_discovery.PlaybackAddress)
	liveHandler := handler.NewLiveHandler()

	liveController := controller.NewLiveController(messageSender, datagrams, liveHandler)

	wg.Add(1)
	go liveController.Run(wg)
}

func main() {
	fmt.Println("playback-caze is runnning!")

	wg := sync.WaitGroup{}
	runPlayback(&wg)
	wg.Wait()
}

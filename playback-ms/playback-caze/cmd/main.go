package main

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/internal/controller"
	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/repository"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func runPlayback(wg *sync.WaitGroup) {
	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.PerceptionAddress,
		service_discovery.PerceptionDetectionWrapperTopic,
	)

	refereeSocket := network.NewZmqSubscriberSocket(
		service_discovery.RefereeAddress,
		service_discovery.RefereeGameStatusTopic,
	)

	messageReceiver := receiver.NewMessageReceiver(
		[]*network.ZmqSubscriberSocket{
			perceptionSocket,
			refereeSocket,
		},
	)

	datagrams := concurrency.NewQueue[network.ZmqMultipartDatagram]()

	messageReceiver.Start(datagrams, wg)
	messageSender := sender.NewMessageSender(service_discovery.PlaybackAddress)

	sampleRepository := repository.NewSampleRepository("redis")
	liveHandler := handler.NewLiveHandler(sampleRepository)
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

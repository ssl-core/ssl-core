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

func enqueueDatagram(queue *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram]) func(datagram network.ZmqMultipartDatagram) {
	return func(datagram network.ZmqMultipartDatagram) {
		queue.Enqueue(datagram)
	}
}

func runPlayback(wg *sync.WaitGroup) {
	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.PerceptionAddress,
		service_discovery.PerceptionDetectionWrapperTopic,
	)

	refereeSocket := network.NewZmqSubscriberSocket(
		service_discovery.RefereeAddress,
		service_discovery.RefereeGameStatusTopic,
	)

	replayRouter := network.NewZmqRouterSocket(
		service_discovery.GatewayReplayChunckAddress,
	)

	subscriberDatagrams := concurrency.NewQueue[network.ZmqMultipartDatagram]()
	routerDatagrams := concurrency.NewQueue[network.ZmqMultipartDatagram]()
	messageReceiver := receiver.NewMessageReceiver(
		[]*receiver.SocketHandler{
			receiver.NewSocketHandler(perceptionSocket, enqueueDatagram(subscriberDatagrams)),
			receiver.NewSocketHandler(refereeSocket, enqueueDatagram(subscriberDatagrams)),
			receiver.NewSocketHandler(replayRouter, enqueueDatagram(routerDatagrams)),
		},
	)

	messageReceiver.Start(wg)
	messageSender := sender.NewMessageSender(service_discovery.PlaybackAddress, replayRouter)

	repositoryFactory := repository.GetRepositoryFactory("redis")
	sampleRepository := repositoryFactory.MakeSampleRepository()

	liveHandler := handler.NewLiveHandler()
	liveController := controller.NewLiveController(messageSender, subscriberDatagrams, liveHandler, sampleRepository)

	replayHandler := handler.NewReplayHandler()
	replayController := controller.NewReplayController(messageSender, routerDatagrams, replayHandler, sampleRepository)

	wg.Add(2) // liveController and replayController
	go liveController.Run(wg)
	go replayController.Run(wg)
}

func main() {
	fmt.Println("playback-caze is runnning!")

	wg := sync.WaitGroup{}
	runPlayback(&wg)

	wg.Wait()
}

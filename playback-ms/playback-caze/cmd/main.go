package main

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/internal/stub"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func runPlayback(wg *sync.WaitGroup) {
	// chunkRequestsChannel := make(chan network.ZmqMultipartDatagram, 10)

	// messageSender := sender.NewMessageSender(
	// 	service_discovery.GetInstance().GetPlaybackAddress(),
	// 	service_discovery.GetInstance().GetChunkAddress(),
	// )

	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.GetInstance().GetPerceptionAddress(),
		service_discovery.GetInstance().GetDetectionWrapperTopic(),
	)

	refereeSocket := network.NewZmqSubscriberSocket(
		service_discovery.GetInstance().GetRefereeAddress(),
		service_discovery.GetInstance().GetRefereeTopic(),
	)

	subscribersDatagramsChannel := make(chan network.ZmqMultipartDatagram, 10)
	message_receiver := receiver.NewMessageReceiver(subscribersDatagramsChannel, []*network.ZmqSubscriberSocket{perceptionSocket, refereeSocket})
	fmt.Println("message receiver before start")
	message_receiver.Start(wg)

	// liveHandler := handler.NewLiveHandler()
	// liveController := controller.NewLiveController(
	// 	messageSender,
	// 	subscribersDatagramsChannel,
	// 	liveHandler,
	// )
	// go liveController.Run(wg)

	// chunkController := controller.NewChunkController(messageSender, chunkRequestsChannel)
	// go chunkController.Run(wg)
}

func runPerceptionStub(wg *sync.WaitGroup) {
	perceptionStub := stub.NewPerceptionStub()
	perceptionStub.Run(wg)
}

func runSubscriberStub(wg *sync.WaitGroup) {
	subscriberStub := stub.NewSubscriberStub()
	subscriberStub.Run(wg)
}

func runGatewayChunkStub(wg *sync.WaitGroup) {
	GatewayChunkStub := stub.NewGatewayChunkStub()
	GatewayChunkStub.Run(wg)
}

func main() {
	fmt.Println("Starting playback-ms...")

	wg := sync.WaitGroup{}
	runPlayback(&wg)
	wg.Wait()
}

package main

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/controller"
	"github.com/robocin/ssl-core/playback-ms/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/internal/stub"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func runPlayback(wg *sync.WaitGroup) {
	wg.Add(1)
	// chunkRequestsChannel := make(chan network.ZmqMultipartDatagram, 10)

	messageSender := sender.NewMessageSender(
		service_discovery.GetInstance().GetPlaybackAddress(),
		service_discovery.GetInstance().GetChunkAddress(),
	)

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

	liveHandler := handler.NewLiveHandler()
	liveController := controller.NewLiveController(
		messageSender,
		subscribersDatagramsChannel,
		liveHandler,
	)
	go liveController.Run(wg)

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

func stubRun() {
	wg := sync.WaitGroup{}
	wg.Add(2)

	go runPerceptionStub(&wg)
	go runSubscriberStub(&wg)

	wg.Wait()
}

func debugRun() {
	wg := sync.WaitGroup{}
	wg.Add(2)

	go runPerceptionStub(&wg)
	go runPlayback(&wg)

	// Wait 2 seconds before starting the GatewayChunkStub to ensure that the
	// perceptionStub has already started and is ready to receive messages
	time.Sleep(2 * time.Second)

	go runGatewayChunkStub(&wg)

	wg.Wait()
}

func main() {
	fmt.Println("Starting playback-ms...")
	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.GetInstance().GetPerceptionAddress(),
		service_discovery.GetInstance().GetDetectionWrapperTopic(),
	)
	for {
		msg := perceptionSocket.Receive()
		if !msg.IsEmpty() {
			fmt.Println("Received 1")
			continue
		}
	}

	// wg := sync.WaitGroup{}
	// runPlayback(&wg)
	// wg.Wait()
}

package main

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/controller"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/internal/stub"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func makePerceptionSubscriber(channel chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		"PerceptionSubscriber",
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetPerceptionAddress(),
			service_discovery.GetInstance().GetDetectionWrapperTopic(),
		),
		channel,
	)
}

func makeRefereeSubscriber(channel chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		"RefereeSubscriber",
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetRefereeAddress(),
			service_discovery.GetInstance().GetRefereeTopic(),
		),
		channel,
	)
}

func runPlayback(wg *sync.WaitGroup) {
	defer wg.Done()
	wg.Add(1)

	subscribersDatagramsChannel := make(chan network.ZmqMultipartDatagram, 10)
	chunkRequestsChannel := make(chan network.ZmqMultipartDatagram, 10)

	sampleControllerUnsavedSamplesChannel := make(chan entity.Sample, 10)
	world.GetInstance().Setup(sampleControllerUnsavedSamplesChannel)

	message_sender := sender.NewMessageSender()
	message_sender.AddPublisher(service_discovery.GetInstance().GetPlaybackAddress(), sender.LivePublisherID)
	router := message_sender.AddRouter(service_discovery.GetInstance().GetChunkAddress(), sender.ChunkRouterID)

	message_receiver := receiver.NewMessageReceiver()
	message_receiver.AddSubscriberHandler(makePerceptionSubscriber(subscribersDatagramsChannel))
	message_receiver.AddSubscriberHandler(makeRefereeSubscriber(subscribersDatagramsChannel))
	message_receiver.AddRouterHandler(handler.NewRouterHandler("ChunkRouter", router, chunkRequestsChannel))
	go message_receiver.Start(wg)

	sampleController := controller.NewSampleController(
		message_sender,
		subscribersDatagramsChannel,
		sampleControllerUnsavedSamplesChannel,
	)
	go sampleController.Run(wg)

	chunkController := controller.NewChunkController(message_sender, chunkRequestsChannel)
	go chunkController.Run(wg)

	wg.Wait()
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

func playbackRun() {
	wg := sync.WaitGroup{}
	wg.Add(1)

	go runPlayback(&wg)

	wg.Wait()
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
	// DEBUG
	// if args := os.Args; len(args) > 1 {
	// 	fmt.Printf("Starting playback-ms [MODE %v]...\n", args[1])
	// 	switch args[1] {
	// 	case "playback":
	// 		playbackRun()
	// 	case "stub":
	// 		stubRun()
	// 	case "debug":
	// 		debugRun()
	// 	default:
	// 		fmt.Println("Unknown argument:", args[1])
	// 	}
	// } else {
	// 	fmt.Println("No arguments provided. Running `playbackRun` by default.")
	// 	playbackRun()
	// }

	fmt.Println("Starting playback-ms...")
	playbackRun()
}

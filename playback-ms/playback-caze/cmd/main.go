package main

import (
	"sync"

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

func makePerceptionSubscriber(channel *chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		"PerceptionSubscriber",
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetPerceptionAddress(),
			service_discovery.GetInstance().GetDetectionWrapperTopic(),
		),
		channel,
	)
}

func makeRefereeSubscriber(channel *chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		"RefereeSubscriber",
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetRefereeAddress(),
			service_discovery.GetInstance().GetRefereeTopic(),
		),
		channel,
	)
}

func main() {
	wg := sync.WaitGroup{}
	wg.Add(3)

	sampleControllerDatagramsChannel := make(chan network.ZmqMultipartDatagram)
	sampleControllerUnsavedSamplesChannel := make(chan entity.Sample)
	chunkControllerChannel := make(chan network.ZmqMultipartDatagram)

	world.GetInstance().Init(sampleControllerUnsavedSamplesChannel)

	message_sender := sender.NewMessageSender()
	message_sender.AddPublisher(service_discovery.GetInstance().GetPerceptionAddress(), sender.LivePublisherID)
	router := message_sender.AddRouter(service_discovery.GetInstance().GetChunkAddress(), sender.ChunkRouterID)

	message_receiver := receiver.NewMessageReceiver()
	message_receiver.AddSubscriberHandler(makePerceptionSubscriber(&sampleControllerDatagramsChannel))
	message_receiver.AddSubscriberHandler(makeRefereeSubscriber(&sampleControllerDatagramsChannel))
	message_receiver.AddRouterHandler(handler.NewRouterHandler("ChunkRouter", router, &chunkControllerChannel))
	message_receiver.Start()

	sampleController := controller.NewSampleController(
		message_sender,
		sampleControllerDatagramsChannel,
		sampleControllerUnsavedSamplesChannel,
	)
	go sampleController.Run(&wg)

	chunkController := controller.NewChunkController(message_sender, chunkControllerChannel)
	go chunkController.Run(&wg)

	perceptionStub := stub.NewPerceptionStub()
	go perceptionStub.Run(&wg)

	wg.Wait()
}

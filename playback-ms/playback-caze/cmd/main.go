package main

import (
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func makePerceptionSubscriber(channel *chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetPerceptionAddress(),
			service_discovery.GetInstance().GetDetectionWrapperTopic(),
		),
		channel,
	)
}

func makeRefereeSubscriber(channel *chan network.ZmqMultipartDatagram) *handler.SubscriberHandler {
	return handler.NewSubscriberHandler(
		network.NewZmqSubscriberSocket(
			service_discovery.GetInstance().GetRefereeAddress(),
			service_discovery.GetInstance().GetRefereeTopic(),
		),
		channel,
	)
}

func main() {
	// wg := sync.WaitGroup{}
	// wg.Add(2)

	// sampleControllerChannel := make(chan network.ZmqMultipartDatagram)
	// chunkControllerChannel := make(chan network.ZmqMultipartDatagram)

	// message_sender := sender.NewMessageSender()
	// message_sender.AddPublisher(service_discovery.GetInstance().GetPerceptionAddress(), sender.LivePublisherID)
	// router := message_sender.AddRouter(service_discovery.GetInstance().GetChunkAddress(), sender.ChunkRouterID)

	// message_receiver := receiver.NewMessageReceiver()
	// message_receiver.AddSubscriberHandler(makePerceptionSubscriber(&sampleControllerChannel))
	// message_receiver.AddSubscriberHandler(makeRefereeSubscriber(&sampleControllerChannel))
	// message_receiver.AddRouterHandler(handler.NewRouterHandler(router, &chunkControllerChannel))
	// message_receiver.Start()

	// sampleController := controller.NewSampleController(message_sender, &sampleControllerChannel)
	// go sampleController.Run(&wg)

	// chunkController := controller.NewChunkController(message_sender, &chunkControllerChannel)
	// go chunkController.Run(&wg)

	// wg.Wait()

	redis_experiment()
}

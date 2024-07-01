package main

import (
	"encoding/json"
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db"
	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/controller"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver/handler"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
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

func databaseExperimentSetGet(db_client db.DBClient) {
	fmt.Printf("Starting databaseExperimentSetGet...\n")
	defer fmt.Printf("-------------------------------\n")

	start := 1
	end := 10

	for i := start; i <= end; i++ {
		db_client.Set(timeAddingSeconds(i), fmt.Sprintf("value_%d", i))
	}

	for i := start; i <= end; i++ {
		key := timeAddingSeconds(i)
		result, err := db_client.Get(key)
		if err != nil {
			fmt.Printf("Error getting value for KEY(%d): %v\n", i, err)
			continue
		}

		values, err := json.Marshal(result)
		if err != nil {
			fmt.Printf("Error marshalling value for KEY(%d): %v\n", i, err)
			continue
		}
		fmt.Printf("KEY(%d) == %s\n", i, string(values))
	}
}

func databaseExperimentSetGetRange(db_client db.DBClient) {
	fmt.Printf("Starting databaseExperimentSetGetRange...\n")
	defer fmt.Printf("-------------------------------\n")

	start := 20
	end := 1000
	for i := start; i <= end; i++ {
		db_client.Set(timeAddingSeconds(i), fmt.Sprintf("value = %d", i))
	}

	start_second := 50
	end_second := 100
	start_time := timeAddingSeconds(start_second)
	end_time := timeAddingSeconds(end_second)
	values, err := db_client.GetChunk(start_time, end_time)
	if err != nil {
		fmt.Printf("Error getting chunk from %d to %d: %v\n", start, end, err)
	}
	fmt.Printf("Chunk from %ds to %ds: %v\n", start_second, end_second, values)
}

func clearDatabase(db_client db.DBClient) {
	db_client.Clear()
}

func timeAddingSeconds(seconds int) time.Time {
	return time.Date(2024, time.February, 1, 0, 0, 0, 0, time.UTC).Add(time.Duration(seconds) * time.Second)
}

func databaseExperiment() {
	redis_client := redis.NewRedisClient("test")
	clearDatabase(redis_client)
	databaseExperimentSetGet(redis_client)
	databaseExperimentSetGetRange(redis_client)
}

func main() {
	wg := sync.WaitGroup{}
	wg.Add(2)

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
	message_receiver.AddRouterHandler(handler.NewRouterHandler(router, &chunkControllerChannel))
	message_receiver.Start()

	sampleController := controller.NewSampleController(
		message_sender,
		sampleControllerDatagramsChannel,
		sampleControllerUnsavedSamplesChannel,
	)
	go sampleController.Run(&wg)

	chunkController := controller.NewChunkController(message_sender, chunkControllerChannel)
	go chunkController.Run(&wg)

	wg.Wait()
}

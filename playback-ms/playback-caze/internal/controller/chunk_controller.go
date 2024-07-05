package controller

import (
	"encoding/json"
	"fmt"
	"log"
	"sync"
	"time"

	"github.com/redis/go-redis/v9"
	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/gateway"
	"google.golang.org/protobuf/proto"
)

const (
	ChunkStream    = "chunk"
	chunkTimeRange = 30 * time.Millisecond
)

type ChunkController struct {
	db_client *redis_db.RedisClient
	channel   chan network.ZmqMultipartDatagram
	sender    *sender.MessageSender
}

func NewChunkController(sender *sender.MessageSender, channel chan network.ZmqMultipartDatagram) *ChunkController {
	return &ChunkController{
		db_client: redis_db.NewRedisClient(ChunkStream),
		channel:   channel,
		sender:    sender,
	}
}

func (cc *ChunkController) Run(wg *sync.WaitGroup) {
	defer fmt.Printf("Finishing ChunkController\n")
	defer wg.Done()

	fmt.Printf("Running ChunkController...\n")
	for datagram := range cc.channel {
		go cc.chunkWorker(datagram)
	}
}

func (cc *ChunkController) chunkWorker(datagram network.ZmqMultipartDatagram) {
	// Get latest sample from world to build GetReplayChunkResponse with latest timestamp.
	sample, err := world.GetInstance().GetLatestSample()
	if err != nil {
		fmt.Printf("Error getting latest sample on ChunkWorker: %v\n", err)
		return
	}

	// Parse GetReplayChunkRequest message.
	var replayChunkRequest gateway.GetReplayChunkRequest
	err = proto.Unmarshal(datagram.Message, &replayChunkRequest)
	if err != nil {
		fmt.Printf("Error unmarshalling GetReplayChunkRequest: %v\n", err)
		return
	}

	// Define the time range for the chunk.
	startTime := replayChunkRequest.StartTimestamp.AsTime()
	endTime := startTime.Add(chunkTimeRange)

	// Get chunk from database and parse the result into a slice of samples.
	chunkValues := cc.db_client.GetChunk(startTime, endTime)
	redisMessages, ok := chunkValues.([]redis.XMessage)
	if !ok {
		fmt.Println("Error: chunkValues is not of type []redis.XMessage")
		return
	}
	parseRedisResultValues(redisMessages)
	chunkSamples := parseRedisResultValues(redisMessages)

	// Send chunk to gateway.
	chunk := entity.NewChunk((*sample).Timestamp, chunkSamples)
	cc.sender.SendChunk(*chunk, string(datagram.Identifier))
}

func parseRedisResultValues(redisMessages []redis.XMessage) []entity.Sample {
	samples := make([]entity.Sample, 0)
	for _, redisMessage := range redisMessages {
		values := redisMessage.Values
		if value, ok := values["value"]; ok {
			var sample entity.Sample
			err := json.Unmarshal([]byte(value.(string)), &sample)
			if err != nil {
				log.Printf("Error unmarshalling JSON: %v", err)
				continue
			}
			samples = append(samples, sample)
		}
	}
	return samples
}

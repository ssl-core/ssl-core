package controller

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
)

const (
	ChunkStream = "chunk"
)

type ChunkController struct {
	db_client *redis.RedisClient
	channel   chan network.ZmqMultipartDatagram
	sender    *sender.MessageSender
}

func NewChunkController(sender *sender.MessageSender, channel chan network.ZmqMultipartDatagram) *ChunkController {
	return &ChunkController{
		db_client: redis.NewRedisClient(ChunkStream),
		channel:   channel,
		sender:    sender,
	}
}

func (cc *ChunkController) Run(wg *sync.WaitGroup) {
	defer fmt.Printf("Finishing ChunkController\n")
	defer wg.Done()

	fmt.Printf("Running ChunkController...\n")
	for {
		datagram := <-cc.channel
		go cc.chunkWorker(datagram)
	}
}

func (cc *ChunkController) chunkWorker(datagram network.ZmqMultipartDatagram) {
	fmt.Println(string(datagram.Message))
	sample, err := world.GetInstance().GetLatestSample()
	if err != nil {
		return
	}
	/*
		message GetReplayChunkRequest {
		  google.protobuf.Timestamp start_timestamp = 1;
		}
	*/
	// TODO(matheusvtna): Decode GetReplayChunkRequest message, get the start_timestamp, get chunk from database and send the chunk
	// message := datagram.Message
	// var replayChunkRequest entity.GetReplayChunkRequest
	// err = proto.Unmarshal(message, &replayChunkRequest)
	// samples := cc.db_client.GetChunk()
	chunk := entity.NewChunk((*sample).Timestamp, make([]entity.Sample, 0))
	cc.sender.SendChunk(*chunk, string(datagram.Identifier))
}

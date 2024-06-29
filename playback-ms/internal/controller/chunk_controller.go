package controller

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	messaging "github.com/robocin/ssl-core/playback-ms/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type ChunkController struct {
	db_client *redis.RedisClient
	channel   *chan network.ZmqMultipartDatagram
	sender    *messaging.MessageSender
}

func NewChunkController(sender *messaging.MessageSender, channel *chan network.ZmqMultipartDatagram) *ChunkController {
	return &ChunkController{
		db_client: redis.NewRedisClient(),
		channel:   channel,
		sender:    sender,
	}
}

func (cc *ChunkController) Run(wg *sync.WaitGroup) {
	defer fmt.Printf("Finishing ChunkController\n")
	defer wg.Done()

	fmt.Printf("Running ChunkController...\n")
	for {
		datagram := <-(*cc.channel)
		go cc.chunk_worker(datagram)
	}
}

func (cc *ChunkController) chunk_worker(datagram network.ZmqMultipartDatagram) {
	for {
		fmt.Println(string(datagram.Message))
		// TODO(matheusvtna): Implement database management to fetch chunks.
		chunk := entity.NewChunk(time.Now(), make([]entity.Sample, 0))
		cc.sender.SendChunk(*chunk, string(datagram.Identifier))
	}
}

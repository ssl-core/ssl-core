package controller

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type ChunkController struct {
	db_client *redis.RedisClient
	channel   *chan network.ZmqMultipartDatagram
	router    *network.ZmqRouterSocket
}

func NewChunkController(router *network.ZmqRouterSocket, channel *chan network.ZmqMultipartDatagram) *ChunkController {
	cc := &ChunkController{
		db_client: redis.NewRedisClient(),
		channel:   channel,
		router:    router,
	}
	return cc
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
		cc.router.Send(datagram)
	}
}

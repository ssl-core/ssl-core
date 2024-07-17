package controller

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/common/golang/concurrency"
	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/common/golang/time_util"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/handler"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/messaging/sender"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/repository"
	"github.com/robocin/ssl-core/protocols/gateway"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

const (
	chunkSize = 5 * time.Second
)

type ReplayController struct {
	sender           *sender.MessageSender
	datagrams        *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram]
	handler          *handler.ReplayHandler
	sampleRepository repository.ISampleRepository
}

func NewReplayController(
	sender *sender.MessageSender,
	datagrams *concurrency.ConcurrentQueue[network.ZmqMultipartDatagram],
	handler *handler.ReplayHandler,
	sampleRepository repository.ISampleRepository,
) *ReplayController {
	return &ReplayController{
		datagrams:        datagrams,
		sender:           sender,
		handler:          handler,
		sampleRepository: sampleRepository,
	}
}

func (rc *ReplayController) responseFor(chunkRequest *gateway.GetReplayChunkRequest) (*gateway.GetReplayChunkResponse, error) {
	start := chunkRequest.GetStartTimestamp()
	end := timestamppb.New(time_util.TimeFromTimestamp(start).Add(chunkSize))
	samples, err := rc.sampleRepository.GetSamples(start, end)
	if err != nil {
		return nil, err
	}
	latestSample, err := rc.sampleRepository.GetLatestSample()
	if err != nil {
		return nil, err
	}
	response := &gateway.GetReplayChunkResponse{
		LastTimestamp: latestSample.Timestamp,
		Samples:       samples,
	}
	return response, nil
}

func (rc *ReplayController) Run(wg *sync.WaitGroup) {
	defer wg.Done()

	for {
		for _, datagram := range rc.datagrams.DequeueAll() {
			if chunkRequest, err := rc.handler.ProcessIfReplayChunk(&datagram); err == nil {
				start := time.Now()
				response, err := rc.responseFor(chunkRequest)
				elapsed := time.Since(start)
				if err != nil {
					fmt.Printf("Failed to handle chunk request: %v\n", err)
					continue
				}
				fmt.Println("total:", len(response.GetSamples()), "entries, time:", elapsed)

				message, err := proto.Marshal(response)

				if err != nil {
					fmt.Printf("failed to marshal 'GetReplayChunkResponse' message: %v\n", err)
					continue
				}

				rc.sender.SendReplayResponse(message, datagram.Identifier)
			}

			if gameEventsRequest, err := rc.handler.ProcessIfGameEvents(&datagram); err == nil {
				response, err := rc.responseForGameEvents(gameEventsRequest)

				if err != nil {
					fmt.Printf("Failed to handle game events request: %v\n", err)
					continue
				}

				rc.sender.SendReplayResponse(response, datagram.Identifier)
			}
		}
	}
}

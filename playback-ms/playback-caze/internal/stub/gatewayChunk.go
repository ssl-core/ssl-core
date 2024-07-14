package stub

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/gateway"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

const (
	chunkRequestFrequencyHz float32 = 0.5
)

type GatewayChunkStub struct {
	ticker *time.Ticker
	dealer *network.ZmqDealerSocket
}

func NewGatewayChunkStub() *GatewayChunkStub {
	return &GatewayChunkStub{
		ticker: time.NewTicker(time.Millisecond / time.Duration(chunkRequestFrequencyHz*1000)),
		dealer: network.NewZmqDealerSocket(service_discovery.GatewayReplayChunckAddress),
	}
}

func (gcs *GatewayChunkStub) Run(wg *sync.WaitGroup) {
	defer wg.Done()

	fmt.Printf("GatewayChunkStub running...\n")

	for range gcs.ticker.C {
		chunkRequest := gateway.GetReplayChunkRequest{
			StartTimestamp: timestamppb.New(time.Now().Add(-time.Second * 1)),
		}
		chunkRequestBytes, err := proto.Marshal(&chunkRequest)
		if err != nil {
			fmt.Printf("Failed to marshal chunk request: %v\n", err)
			continue
		}
		data := *network.NewZmqDatagram(chunkRequestBytes)
		fmt.Printf("gcs Router Sending chunk request: %v\n", chunkRequest.String())
		gcs.dealer.Send(data)
		response := gcs.dealer.Receive()
		var replayResponse gateway.GetReplayChunkResponse
		err = proto.Unmarshal(response.Message, &replayResponse)
		if err != nil {
			fmt.Printf("Failed to unmarshal chunk response: %v\n", err)
			continue
		}
		fmt.Println("gcs Router Received chunk: ", replayResponse.String())
	}
}

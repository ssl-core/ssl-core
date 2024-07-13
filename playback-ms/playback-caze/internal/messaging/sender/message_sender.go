package sender

import (
	"fmt"
	"log"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/gateway"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/proto"
)

type MessageSender struct {
	publisher *network.ZmqPublisherSocket
	router    *network.ZmqRouterSocket
	count     uint64 // TODO: remove it later.
}

func NewMessageSender(addressPublisher string, router *network.ZmqRouterSocket) *MessageSender {
	return &MessageSender{
		publisher: network.NewZmqPublisherSocket(addressPublisher),
		router:    router,
	}
}

func (ms *MessageSender) SendSample(sample *playback.Sample) {
	message, err := proto.Marshal(sample)

	if err != nil {
		log.Fatalf("failed to marshal 'Sample' message: %v", err)
		return
	}

	datagram := network.ZmqMultipartDatagram{
		Identifier: []byte(service_discovery.GatewayLivestreamTopic),
		Message:    message,
	}

	ms.count++
	fmt.Println(ms.count, "samples sent.")
	ms.publisher.Send(datagram)
}

func (ms *MessageSender) SendReplayResponse(chunk *gateway.GetReplayChunkResponse, identifier []byte) {
	message, err := proto.Marshal(chunk)

	if err != nil {
		log.Fatalf("failed to marshal 'GetReplayChunkResponse' message: %v", err)
		return
	}
	datagram := network.ZmqMultipartDatagram{
		Identifier: identifier,
		Message:    message,
	}

	fmt.Printf("replay response sent to %s\n", string(identifier))
	ms.router.Send(datagram)
}

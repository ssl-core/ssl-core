package sender

import (
	"fmt"
	"log"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/proto"
)

type MessageSender struct {
	publisher *network.ZmqPublisherSocket
	// router    *network.ZmqRouterSocket // TODO: add router again.
	count uint64 // TODO: remove it later.
}

func NewMessageSender(addressPublisher string) *MessageSender {
	return &MessageSender{
		publisher: network.NewZmqPublisherSocket(addressPublisher),
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

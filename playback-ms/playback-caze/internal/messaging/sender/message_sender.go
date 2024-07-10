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
	router    *network.ZmqRouterSocket
}

func NewMessageSender(addressPublisher string, addressRouter string) *MessageSender {
	return &MessageSender{
		publisher: network.NewZmqPublisherSocket(addressPublisher),
		router:    network.NewZmqRouterSocket(addressRouter),
	}
}

func (ms *MessageSender) SendSample(sample *playback.Sample) {
	messageBytes, err := proto.Marshal(sample)
	if err != nil {
		log.Fatalf("Failed to marshal Sample message: %v", err)
	}
	datagram := network.ZmqMultipartDatagram{
		Identifier: []byte(service_discovery.GetInstance().GetLivePublishTopic()),
		Message:    messageBytes,
	}

	fmt.Println("Sending sample: ", sample.String())
	ms.publisher.Send(datagram)
}

// func (ms *MessageSender) SendChunk(chunk entity.Chunk, request_id string) {
// 	router := ms.routers[ChunkRouterID]
// 	if router == nil {
// 		log.Fatalf("Router %s not found", ChunkRouterID)
// 		return
// 	}

// 	chunkBytes, err := proto.Marshal(chunk.ToProto())
// 	if err != nil {
// 		log.Fatalf("Failed to marshal Chunk message: %v", err)
// 	}
// 	datagram := network.ZmqMultipartDatagram{
// 		Identifier: []byte(request_id),
// 		Message:    chunkBytes,
// 	}
// 	router.Send(datagram)
// }

// func (ms *MessageSender) SendGameEvents(gameEvents *entity.GameEvents, request_id string) {
// 	router := ms.routers[ChunkRouterID]
// 	if router == nil {
// 		log.Fatalf("Router %s not found", ChunkRouterID)
// 		return
// 	}

// 	gameEventsBytes, err := proto.Marshal(gameEvents.ToProto())
// 	if err != nil {
// 		log.Fatalf("Failed to marshal GameEvents message: %v", err)
// 	}
// 	datagram := network.ZmqMultipartDatagram{
// 		Identifier: []byte(request_id),
// 		Message:    gameEventsBytes,
// 	}
// 	router.Send(datagram)
// }

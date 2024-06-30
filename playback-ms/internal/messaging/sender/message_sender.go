package sender

import (
	"log"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/internal/world"
	"github.com/robocin/ssl-core/playback-ms/network"
	"google.golang.org/protobuf/proto"
)

const (
	LivePublisherID = "livePublisher"
	ChunkRouterID   = "chunkRouter"
)

type MessageSender struct {
	publishers map[string]*network.ZmqPublisherSocket
	routers    map[string]*network.ZmqRouterSocket
}

func (ms *MessageSender) AddPublisher(address string, id string) *network.ZmqPublisherSocket {
	if ms.publishers == nil {
		ms.publishers = make(map[string]*network.ZmqPublisherSocket)
	}

	if pub, ok := ms.publishers[id]; ok {
		return pub
	}
	pub := network.NewZmqPublisherSocket(address)
	ms.publishers[id] = pub
	return pub
}

func (ms *MessageSender) AddRouter(address string, id string) *network.ZmqRouterSocket {
	if ms.routers == nil {
		ms.routers = make(map[string]*network.ZmqRouterSocket)
	}

	if router, ok := ms.routers[id]; ok {
		return router
	}
	router := network.NewZmqRouterSocket(address)
	ms.routers[id] = router
	return router
}

func NewMessageSender() *MessageSender {
	return &MessageSender{}
}

func (ms *MessageSender) SendSample(sample entity.Sample) {
	publisher := ms.publishers[LivePublisherID]
	if publisher == nil {
		log.Fatalf("Publisher %s not found", LivePublisherID)
		return
	}

	message := sample.ToProto()
	messageBytes, err := proto.Marshal(message)
	if err != nil {
		log.Fatalf("Failed to marshal Sample message: %v", err)
	}
	datagram := network.ZmqMultipartDatagram{
		Identifier: []byte(service_discovery.GetInstance().GetLivePublishTopic()),
		Message:    messageBytes,
	}
	publisher.Send(datagram)
}

func (ms *MessageSender) SendChunk(chunk entity.Chunk, request_id string) {
	router := ms.routers[ChunkRouterID]
	if router == nil {
		log.Fatalf("Router %s not found", ChunkRouterID)
		return
	}

	sample, err := world.GetInstance().GetLatestSample()
	if err != nil {
		log.Fatalf("Failed to get latest sample: %v", err)
	}

	message, err := chunk.ToProto(sample.Timestamp)
	if err != nil {
		log.Fatalf("Failed to convert Chunk to Proto: %v", err)
	}
	messageBytes, err := proto.Marshal(message)
	if err != nil {
		log.Fatalf("Failed to marshal Chunk message: %v", err)
	}
	datagram := network.ZmqMultipartDatagram{
		Identifier: []byte(request_id),
		Message:    messageBytes,
	}
	router.Send(datagram)
}

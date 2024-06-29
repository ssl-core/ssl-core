package messaging

import (
	"log"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/network"
	"google.golang.org/protobuf/proto"
)

const (
	livePublishAddress = "ipc:///tmp/gateway-async-frontend.ipc"
	livePublishTopic   = "sample"
	livePublishID      = "livePublisher"

	liveFrequencyHZ = 60
)

type MessageSender struct {
	publishers map[string]*network.ZmqPublisherSocket
}

func (ms *MessageSender) makePublisher(address string, id string) *network.ZmqPublisherSocket {
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

func NewMessageSender() *MessageSender {
	return &MessageSender{}
}

func (ms *MessageSender) Setup() {
	ms.makePublisher(livePublishAddress, livePublishID)
}

func (ms *MessageSender) SendSample(sample entity.Sample) {
	message := sample.ToProto()
	messageBytes, err := proto.Marshal(message)
	if err != nil {
		log.Fatalf("Failed to marshal message: %v", err)
	}
	datagram := network.ZmqMultipartDatagram{
		Identifier: []byte(livePublishTopic),
		Message:    messageBytes,
	}
	ms.publishers[livePublishID].Send(datagram)
}

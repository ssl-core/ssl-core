package main

import (
	"log"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/transport"
	"google.golang.org/protobuf/proto"
)

const (
	perceptionAddress     = "ipc:///tmp/perception.ipc"
	detectionWrapperTopic = "wrapper"
	refereeAddress        = "ipc:///tmp/referee.ipc"
	refereeTopic          = "referee"

	livePublishAddress = "ipc:///tmp/gateway-async-frontend.ipc"
	livePublishTopic   = "sample"
	livePublishID      = "livePublisher"

	liveFrequencyHZ = 60
)

// MessageReceiver

type MessageReceiver struct{}

func NewMessageReceiver() *MessageReceiver {
	return &MessageReceiver{}
}

func (mr *MessageReceiver) makeSubscriber(address, topic string) *transport.ZmqSubscriberSocket {
	return transport.NewZmqSubscriberSocket(address, topic)
}

func (mr *MessageReceiver) receiveDatagrams(subscriber *transport.ZmqSubscriberSocket, datagramChannel chan transport.ZmqMultipartDatagram) {
	defer subscriber.Close()
	for {
		datagram := subscriber.Receive()
		datagramChannel <- datagram
	}
}

func (mr *MessageReceiver) Run(datagramChannel chan transport.ZmqMultipartDatagram) {
	go mr.receiveDatagrams(mr.makeSubscriber(perceptionAddress, detectionWrapperTopic), datagramChannel)
	go mr.receiveDatagrams(mr.makeSubscriber(refereeAddress, refereeTopic), datagramChannel)
}

// MessageSender

type MessageSender struct {
	publishers map[string]*transport.ZmqPublisherSocket
}

func (ms *MessageSender) makePublisher(address string, id string) *transport.ZmqPublisherSocket {
	if ms.publishers == nil {
		ms.publishers = make(map[string]*transport.ZmqPublisherSocket)
	}

	if pub, ok := ms.publishers[id]; ok {
		return pub
	}
	pub := transport.NewZmqPublisherSocket(address)
	ms.publishers[id] = pub
	return pub
}

func NewMessageSender() *MessageSender {
	return &MessageSender{}
}

func (ms *MessageSender) setupPublishers() {
	ms.makePublisher(livePublishAddress, livePublishID)
}

func (ms *MessageSender) sendLatestSample(sample entity.Sample) {
	message := sample.ToProto()
	messageBytes, err := proto.Marshal(message)
	if err != nil {
		log.Fatalf("Failed to marshal message: %v", err)
	}
	datagram := transport.ZmqMultipartDatagram{
		Identifier: []byte(livePublishTopic),
		Message:    messageBytes,
	}
	ms.publishers[livePublishID].Send(datagram)
}

// DB

func saveToDatabase(datagrams []transport.ZmqMultipartDatagram) {
	// save to database
}

// WorldState

type WorldState struct {
	latest_sample entity.Sample
	datagrams     []transport.ZmqMultipartDatagram
	mu            sync.Mutex
	sender        MessageSender
	ticker        *time.Ticker
}

func NewWorldState() *WorldState {
	return &WorldState{
		latest_sample: entity.Sample{},
		datagrams:     make([]transport.ZmqMultipartDatagram, 0),
		sender:        MessageSender{},
		ticker:        time.NewTicker(time.Second / liveFrequencyHZ),
	}
}

func NewWorldStateWithSender(sender MessageSender) *WorldState {
	return &WorldState{
		latest_sample: entity.Sample{},
		datagrams:     make([]transport.ZmqMultipartDatagram, 0),
		sender:        sender,
		ticker:        time.NewTicker(time.Second / liveFrequencyHZ),
	}
}

func (ws *WorldState) updateSample(datagram transport.ZmqMultipartDatagram) {
	ws.mu.Lock()
	defer ws.mu.Unlock()
	ws.datagrams = append(ws.datagrams, datagram)
	ws.latest_sample.UpdateFromDatagram(datagram)
}

func (ws *WorldState) handleDatagramsFrom(datagramChannel chan transport.ZmqMultipartDatagram) {
	go func() {
		for datagram := range datagramChannel {
			ws.updateSample(datagram)
		}
	}()

	for range ws.ticker.C {
		ws.mu.Lock()
		if len(ws.datagrams) > 0 {
			go saveToDatabase(ws.datagrams)
			go ws.sender.sendLatestSample(ws.latest_sample)
			ws.datagrams = nil
		}
		ws.mu.Unlock()
	}
}

func main() {
	datagramChannel := make(chan transport.ZmqMultipartDatagram)

	receiver := MessageReceiver{}
	receiver.Run(datagramChannel)

	sender := MessageSender{}
	sender.setupPublishers()

	worldState := NewWorldStateWithSender(sender)
	worldState.handleDatagramsFrom(datagramChannel)
}

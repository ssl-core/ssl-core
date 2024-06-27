package main

import (
	"fmt"
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

type MessageReceiver struct {
	datagramChannel chan transport.ZmqMultipartDatagram
}

func NewMessageReceiver(datagramChannel chan transport.ZmqMultipartDatagram) *MessageReceiver {
	return &MessageReceiver{
		datagramChannel: datagramChannel,
	}
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

func (mr *MessageReceiver) Run() {
	go mr.receiveDatagrams(mr.makeSubscriber(perceptionAddress, detectionWrapperTopic), mr.datagramChannel)
	go mr.receiveDatagrams(mr.makeSubscriber(refereeAddress, refereeTopic), mr.datagramChannel)
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

func (ms *MessageSender) setup() {
	ms.makePublisher(livePublishAddress, livePublishID)
}

func (ms *MessageSender) sendSample(sample entity.Sample) {
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
	latest_sample   entity.Sample
	datagrams       []transport.ZmqMultipartDatagram
	datagramChannel chan transport.ZmqMultipartDatagram
	mu              sync.Mutex
	wg              *sync.WaitGroup
	sender          MessageSender
	ticker          *time.Ticker
}

func NewWorldState(sender MessageSender, datagramChannel chan transport.ZmqMultipartDatagram) *WorldState {
	wg := sync.WaitGroup{}
	wg.Add(2)

	return &WorldState{
		latest_sample:   entity.Sample{},
		datagrams:       make([]transport.ZmqMultipartDatagram, 0),
		datagramChannel: datagramChannel,
		mu:              sync.Mutex{},
		wg:              &wg,
		sender:          sender,
		ticker:          time.NewTicker(time.Second / liveFrequencyHZ),
	}
}

func (ws *WorldState) updateLatestSample() {
	defer ws.wg.Done()

	for datagram := range ws.datagramChannel {
		ws.mu.Lock()
		defer ws.mu.Unlock()
		ws.datagrams = append(ws.datagrams, datagram)
		ws.latest_sample.UpdateFromDatagram(datagram)
	}
}

// TODO(matheusvtna): Change the name of this function...
func (ws *WorldState) sendLatestSample() {
	defer ws.wg.Done()

	// currentTime := time.Now()
	for range ws.ticker.C {
		// fmt.Printf("Sending latest sample... (deltaT(ms) == %v)\n", time.Since(currentTime).Milliseconds())
		// currentTime = time.Now()
		ws.mu.Lock()
		if len(ws.datagrams) > 0 {
			go saveToDatabase(ws.datagrams)
			go ws.sender.sendSample(ws.latest_sample)
			ws.datagrams = nil
		}
		ws.mu.Unlock()
	}
}

func (ws *WorldState) Run() {
	fmt.Printf("Running WorldState...\n")
	go ws.updateLatestSample()
	go ws.sendLatestSample()
	ws.wg.Wait()

	fmt.Printf("Finishing WorldState\n")
}

func main() {
	datagramChannel := make(chan transport.ZmqMultipartDatagram)

	receiver := NewMessageReceiver(datagramChannel)
	receiver.Run()

	sender := MessageSender{}
	sender.setup()

	worldState := NewWorldState(sender, datagramChannel)
	worldState.Run()
}

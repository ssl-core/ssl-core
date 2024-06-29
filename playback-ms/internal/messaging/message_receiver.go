package messaging

import (
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
)

type MessageReceiver struct {
	subscribersChannel chan network.ZmqMultipartDatagram
}

func NewMessageReceiver(subscribersChannel chan network.ZmqMultipartDatagram) *MessageReceiver {
	return &MessageReceiver{
		subscribersChannel: subscribersChannel,
	}
}

func (mr *MessageReceiver) makeSubscriber(address, topic string) *network.ZmqSubscriberSocket {
	return network.NewZmqSubscriberSocket(address, topic)
}

func (mr *MessageReceiver) handleSubscriber(subscriber *network.ZmqSubscriberSocket) {
	defer subscriber.Close()
	for {
		datagram := subscriber.Receive()
		mr.subscribersChannel <- datagram
	}
}

func (mr *MessageReceiver) Run() {
	go mr.handleSubscriber(mr.makeSubscriber(service_discovery.GetInstance().GetPerceptionAddress(), service_discovery.GetInstance().GetDetectionWrapperTopic()))
	go mr.handleSubscriber(mr.makeSubscriber(service_discovery.GetInstance().GetRefereeAddress(), service_discovery.GetInstance().GetRefereeTopic()))

	// go mr.handleChunkRequests()
}

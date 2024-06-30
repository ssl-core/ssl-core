package receiver

import "github.com/robocin/ssl-core/playback-ms/internal/messaging/receiver/handler"

type MessageReceiver struct {
	subscribers []*handler.SubscriberHandler
	routers     []*handler.RouterHandler
}

func NewMessageReceiver() *MessageReceiver {
	return &MessageReceiver{}
}

func (mr *MessageReceiver) AddSubscriberHandler(subscriber *handler.SubscriberHandler) {
	mr.subscribers = append(mr.subscribers, subscriber)
}

func (mr *MessageReceiver) AddRouterHandler(router *handler.RouterHandler) {
	mr.routers = append(mr.routers, router)
}

func (mr *MessageReceiver) Start() {
	for _, subscriber := range mr.subscribers {
		go subscriber.Handle()
	}

	for _, router := range mr.routers {
		go router.Handle()
	}
}

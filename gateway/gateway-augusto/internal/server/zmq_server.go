package server

import "github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"

type ZmqServer struct {
	publisher network.ZmqPublisherSocket
	proxy     <-chan []byte
}

func NewZqmServer(address string, proxy <-chan []byte) *ZmqServer {
	return &ZmqServer{
		publisher: *network.NewZmqPublisherSocket(address),
		proxy:     proxy,
	}
}

func (server *ZmqServer) Start() {
	for {
		for message := range server.proxy {
			server.publisher.Send(*network.NewZmqDatagram("vision", message))
		}
	}
}

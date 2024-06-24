package server

import (
	"fmt"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
)

type ZmqServer struct {
	publisher network.ZmqPublisherSocket
	proxy     <-chan network.ZmqMultipartDatagram
}

func NewZqmServer(address string, proxy <-chan network.ZmqMultipartDatagram) *ZmqServer {
	return &ZmqServer{
		publisher: *network.NewZmqPublisherSocket(address),
		proxy:     proxy,
	}
}

func (s *ZmqServer) Start() {
	for {
		for datagram := range s.proxy {
			fmt.Println("Receiving...", datagram)
			s.publisher.Send(datagram)
		}
	}
}

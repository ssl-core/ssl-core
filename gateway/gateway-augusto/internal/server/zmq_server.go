package server

import (
	"fmt"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
)

type ZmqServer struct {
	Publisher network.ZmqPublisherSocket
	Proxy     <-chan network.ZmqDatagram
}

func NewZqmServer(address string, proxy <-chan network.ZmqDatagram) *ZmqServer {
	return &ZmqServer{
		Publisher: *network.NewZmqPublisherSocket(address),
		Proxy:     proxy,
	}
}

func (s *ZmqServer) Start() {
	for {
		for datagram := range s.Proxy {
			fmt.Println("Receiving...", datagram)
			s.Publisher.Send(datagram)
		}
	}
}

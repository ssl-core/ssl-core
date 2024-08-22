package server

import (
	"github.com/robocin/ssl-core/common/golang/network"
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
			s.publisher.Send(datagram)
		}
	}
}

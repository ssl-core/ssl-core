package application

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"

	"github.com/robocin/ssl-core/playback-ms/network"
)

type ChunkController struct {
	datagramChannel chan network.ZmqMultipartDatagram
}

func buildChunkResponseFor(requestDatagram network.ZmqMultipartDatagram) network.ZmqMultipartDatagram {
	return network.ZmqMultipartDatagram{
		Identifier: requestDatagram.Identifier,
		Message:    []byte("Hello, World!"),
	}
}

func (cc *ChunkController) handle() {
	router := network.NewZmqRouterSocket(service_discovery.GetInstance().GetChunkAddress())

	for {
		for datagram := range cc.datagramChannel {
			fmt.Println(string(datagram.Identifier))
			response := buildChunkResponseFor(datagram)
			router.Send(response)
		}
	}
}

package main

import (
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/client"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/server"
)

func main() {
	proxy := make(chan []byte)

	tp := client.NewUdpMulticastClient("224.5.23.2:10020", 1024, proxy)
	go tp.Listen()

	zmq := server.NewZqmServer("ipc:///tmp/gateway.ipc", proxy)
	zmq.Start()
}

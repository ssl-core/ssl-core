package main

import (
	"sync"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/server"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/worker"
)

func startGatewayWorker(address string, proxy chan network.ZmqMultipartDatagram, wg *sync.WaitGroup) {
	defer wg.Done()

	w := worker.NewUdpMulticastWorker(address, 1024, proxy, "1")
	w.Listen()
}

func startGatewayZmqServer(proxy chan network.ZmqMultipartDatagram, wg *sync.WaitGroup) {
	defer wg.Done()

	server := server.NewZqmServer("ipc:///tmp/gateway.ipc", proxy)
	server.Start()
}

func startGatewayGrpcServer(wg *sync.WaitGroup) {
	defer wg.Done()

	server := server.NewGrpcServer(":50051")
	server.Start()
}

func main() {
	// TODO(aalmds): refactor addresses with service discovery
	// TODO(aalmds): review error handlers

	wg := sync.WaitGroup{}
	wg.Add(3)

	proxy := make(chan network.ZmqMultipartDatagram)
	go startGatewayWorker("224.5.23.2:10020", proxy, &wg)
	go startGatewayZmqServer(proxy, &wg)
	go startGatewayGrpcServer(&wg)

	wg.Wait()
}

package main

import (
	"fmt"
	"os"
	"sync"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/server"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/worker"
)

func startGatewayUdpMulticastWorker(address string, proxy chan network.ZmqMultipartDatagram, topic string, wg *sync.WaitGroup) {
	defer wg.Done()

	w := worker.NewUdpMulticastWorker(address, 2048, proxy, topic)
	w.Listen()
}

func startGatewayZmqServer(proxy chan network.ZmqMultipartDatagram, wg *sync.WaitGroup) {
	defer wg.Done()

	server := server.NewZqmServer("ipc:///tmp/.ssl-core/gateway.ipc", proxy)
	server.Start()
}

func startGatewayGrpcServer(wg *sync.WaitGroup) {
	defer wg.Done()

	server := server.NewGrpcServer("0.0.0.0:50051")
	server.Start()
}

func main() {
	visionPort := os.Getenv("VISION_PORT")
	if visionPort == "" {
		fmt.Println("VISION_PORT not set")
		return
	}

	gcPort := os.Getenv("GC_PORT")
	if gcPort == "" {
		fmt.Println("GC_PORT not set")
		return
	}

	fmt.Println("gateway-augusto is running!")

	// TODO(aalmds): refactor addresses with service discovery
	// TODO(aalmds): review error handlers

	proxy := make(chan network.ZmqMultipartDatagram)

	wg := sync.WaitGroup{}
	wg.Add(5)
	go startGatewayUdpMulticastWorker(fmt.Sprintf("224.5.23.2:%s", visionPort), proxy, "vision-third-party", &wg)
	go startGatewayUdpMulticastWorker("224.5.23.2:10010", proxy, "tracked-third-party", &wg)
	go startGatewayUdpMulticastWorker(fmt.Sprintf("224.5.23.1:%s", gcPort), proxy, "referee-third-party", &wg)
	go startGatewayZmqServer(proxy, &wg)
	go startGatewayGrpcServer(&wg)

	wg.Wait()
}

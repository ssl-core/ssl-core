package main

import (
	"fmt"
	"os"
	"strconv"
	"sync"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/metrics"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/server"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/socket"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/worker"
)

func startGatewayUdpMulticastWorker(address string, proxy chan network.ZmqMultipartDatagram, topic string, wg *sync.WaitGroup, onReceive func(datagram network.ZmqMultipartDatagram)) {
	defer wg.Done()

	w := worker.NewUdpMulticastWorker(address, 2048, proxy, topic, onReceive)
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

func interceptUDPWorkerWith(timeMeasurement *metrics.TimeMeasurementFromAutoRefInputToPlaybackOutput, address string, proxy chan network.ZmqMultipartDatagram, topic string, wg *sync.WaitGroup) {
	defer wg.Done()

	w := worker.NewUdpMulticastWorker(address, 2048, proxy, topic, timeMeasurement.GetStartTime)
	w.Listen()
}

func interceptGrpcServerWith(timeMeasurement *metrics.TimeMeasurementFromAutoRefInputToPlaybackOutput, wg *sync.WaitGroup) {
	defer wg.Done()

	subscriber := socket.NewSocketReceiverHandler(
		network.NewZmqSubscriberSocket("ipc:///tmp/.ssl-core/gateway.ipc", "sample"),
		timeMeasurement.GetEndTime,
	)
	server := server.NewGrpcServerWithSubscriberHandler("0.0.0.0:50051", *subscriber)
	server.Start()
}

func main() {
	visionAddr := os.Getenv("VISION_ADDRESS")
	if visionAddr == "" {
		fmt.Println("VISION_ADDRESS not set")
		return
	}

	trackedAddr := os.Getenv("TRACKED_ADDRESS")
	if trackedAddr == "" {
		fmt.Println("TRACKED_ADDRESS not set")
		return
	}

	gcAddr := os.Getenv("GC_ADDRESS")
	if gcAddr == "" {
		fmt.Println("GC_ADDRESS not set")
		return
	}

	fmt.Println("gateway-augusto is running!")

	// TODO(aalmds): refactor addresses with service discovery
	// TODO(aalmds): review error handlers

	proxy := make(chan network.ZmqMultipartDatagram)

	wg := sync.WaitGroup{}
	wg.Add(5)

	go startGatewayUdpMulticastWorker(visionAddr, proxy, "vision-third-party", &wg, nil)
	go startGatewayUdpMulticastWorker(gcAddr, proxy, "referee-third-party", &wg, nil)
	go startGatewayZmqServer(proxy, &wg)

	if os.Getenv("INTERCEPT") != "true" {
		fmt.Println("INTERCEPT is set to false, starting without interceptors")

		go startGatewayUdpMulticastWorker(trackedAddr, proxy, "tracked-third-party", &wg, nil)
		go startGatewayGrpcServer(&wg)
	} else {
		fmt.Println("INTERCEPT is set to true, starting interceptors")

		waitPackages := getEnvironmentVariable("WAIT_PACKAGES", func(s string) (interface{}, error) {
			i, err := strconv.Atoi(s)
			return i, err
		})
		measurePackages := getEnvironmentVariable("MEASURE_PACKAGES", func(s string) (interface{}, error) {
			i, err := strconv.Atoi(s)
			return i, err
		})

		fmt.Println("Starting time measurement waiting for", waitPackages, "packages and measuring", measurePackages, "packages")
		timeMeasurement := metrics.NewPipelineTimeMeasurement(waitPackages.(int), measurePackages.(int))

		go interceptUDPWorkerWith(timeMeasurement, trackedAddr, proxy, "tracked-third-party", &wg)
		go interceptGrpcServerWith(timeMeasurement, &wg)
	}

	wg.Wait()
}

func getEnvironmentVariable(key string, conversion func(string) (interface{}, error)) interface{} {
	value := os.Getenv(key)
	if value == "" {
		fmt.Printf("%s not set\n", key)
		return nil
	}

	convertedValue, err := conversion(value)
	if err != nil {
		fmt.Printf("Invalid %s value\n", key)
		return nil
	}
	return convertedValue
}

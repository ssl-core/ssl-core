package main

import (
	"fmt"

	"github.com/robocin/ssl-core/match-bff/api/server"
	"github.com/robocin/ssl-core/match-bff/internal/client"
)

const (
	serverAddress = "0.0.0.0:4040"
	grpcAddress   = "gateway:50051"
	clientType    = "grpc"
)

func main() {
	grpcClient := client.NewGrpcClient(grpcAddress)
	go grpcClient.ReceiveLiveStream()

	fmt.Println("Connected to " + grpcAddress)

	wsServer := server.NewWebsocketServer(grpcClient)
	httpServer := server.NewHttpServer(serverAddress, wsServer, grpcClient)

	fmt.Println("Listening on " + serverAddress)

	if err := httpServer.Serve(); err != nil {
		panic(err)
	}
}

package main

import (
	"fmt"

	"github.com/robocin/ssl-core/match-bff/api/server"
	"github.com/robocin/ssl-core/match-bff/internal/client"
)

const (
	serverAddress = "localhost:8080"
	grpcAddress   = "localhost:50051"
	clientType    = "grpc"
)

func main() {
	grpcClient := client.NewGrpcClient(grpcAddress)
	wsServer := server.NewWebsocketServer(grpcClient)
	httpServer := server.NewHttpServer(serverAddress, wsServer, grpcClient)

	fmt.Println("Listening on " + serverAddress)

	if err := httpServer.Serve(); err != nil {
		panic(err)
	}
}
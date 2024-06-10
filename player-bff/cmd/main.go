package main

import (
	"fmt"

	grpcClient "github.com/robocin/ssl-core/player-bff/transport/grpc/client"
	httpServer "github.com/robocin/ssl-core/player-bff/transport/http/server"
	websocketServer "github.com/robocin/ssl-core/player-bff/transport/websocket/server"
)

const (
	serverAddress = "0.0.0.0:4040"
	grpcAddress   = "gateway:50051"
	clientType    = "grpc"
)

func main() {
	client := grpcClient.NewGrpcClient(grpcAddress)
	fmt.Println("Connected to " + grpcAddress)

	server := httpServer.NewHttpServer(serverAddress)
	wsServer := websocketServer.NewWebsocketServer(server.GetRouter(), client)

	fmt.Println("Listening on " + serverAddress)

	if err := server.Serve(); err != nil {
		panic(err)
	}
}

package main

import (
	"fmt"

	"github.com/robocin/ssl-core/player-bff/internal/application"
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
	sharedProxy := application.NewConnectionProxy()
	client := grpcClient.NewGrpcClient(grpcAddress)

	go client.ReceiveLiveStream(sharedProxy)
	fmt.Println("Connected to " + grpcAddress)

	server := httpServer.NewHttpServer(serverAddress)
	router := server.GetRouter()
	wsServer := websocketServer.NewWebsocketServer(router, client, sharedProxy)

	wsServer.Register()
	fmt.Println("Listening on " + serverAddress)

	if err := server.Serve(); err != nil {
		panic(err)
	}
}

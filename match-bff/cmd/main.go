package main

import (
	"github.com/robocin/ssl-core/match-bff/api/server"
)

const (
	port   = ":8080"
	client = "grpc"
)

func main() {
	wsServer := server.NewWebsocketServer()
	httpServer := server.NewHttpServer(port, wsServer)

	if err := httpServer.Serve(); err != nil {
		panic(err)
	}
}

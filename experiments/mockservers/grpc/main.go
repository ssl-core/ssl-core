package main

import (
	"fmt"
	"log"
	"net"

	pb "mockserver/grpc/pb"
	"mockserver/grpc/services"

	"google.golang.org/grpc"
)

func main() {
	listener, err := net.Listen("tcp", ":8080")
	if err != nil {
		panic(err)
	}

	s := grpc.NewServer()
	pb.RegisterMatchServiceServer(s, &services.MatchService{})
	fmt.Println("Listening.")
	if err := s.Serve(listener); err != nil {
		log.Fatalf("failed to serve: %v", err)
	}
}

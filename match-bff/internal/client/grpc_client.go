package client

import (
	"context"
	"log"

	"github.com/robocin/ssl-core/match-bff/internal/domain"
	pb "github.com/robocin/ssl-core/match-bff/pkg/pb"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

type GrpcClient struct {
	address string
	conn    *grpc.ClientConn
}

func NewGrpcClient(address string) *GrpcClient {
	conn, err := grpc.Dial(address, grpc.WithTransportCredentials(insecure.NewCredentials()))

	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	return &GrpcClient{
		address: address,
		conn:    conn,
	}
}

func (gc *GrpcClient) GetConn() *grpc.ClientConn {
	return gc.conn
}

func (gc *GrpcClient) Close() error {
	return gc.conn.Close()
}

func (gc *GrpcClient) GetMatch() (domain.Match, error) {
	client := pb.NewMatchServiceClient(gc.conn)
	response, err := client.Get(context.Background(), &pb.MatchRequest{
		Req: "test",
	})

	if err != nil {
		return domain.Match{}, err
	}

	match := domain.Match{
		Id:   response.Match.Id,
		Name: response.Match.Name,
	}

	return match, nil
}

package client

import (
	"context"
	"io"
	"log"

	"github.com/robocin/ssl-core/match-bff/internal/domain"
	"github.com/robocin/ssl-core/match-bff/internal/service"
	pb "github.com/robocin/ssl-core/match-bff/pkg/pb/ui"
	"golang.org/x/net/websocket"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/protobuf/types/known/durationpb"
)

type GrpcClient struct {
	address     string
	conn        *grpc.ClientConn
	socketConns map[*websocket.Conn]bool
}

func NewGrpcClient(address string) *GrpcClient {
	opt := grpc.WithTransportCredentials(insecure.NewCredentials())
	conn, err := grpc.Dial(address, opt)

	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	return &GrpcClient{
		address:     address,
		conn:        conn,
		socketConns: make(map[*websocket.Conn]bool),
	}
}

func (gc *GrpcClient) GetConn() interface{} {
	return gc.conn
}

func (gc *GrpcClient) Close() error {
	return gc.conn.Close()
}

func (gc *GrpcClient) SubscribeToLiveStream(conn *websocket.Conn) {
	gc.socketConns[conn] = true
}

func (gc *GrpcClient) UnsubscribeToLiveStream(conn *websocket.Conn) {
	delete(gc.socketConns, conn)
}

func (gc *GrpcClient) ReceiveLiveStream() error {
	client := pb.NewGatewayServiceClient(gc.conn)

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	stream, err := client.ReceiveLiveStream(ctx, &pb.ReceiveLiveStreamRequest{
		MessageTypes: []pb.MessageType{
			pb.MessageType_MESSAGE_TYPE_VISION,
		},
	})

	if err != nil {
		return err
	}

	for {
		response, err := stream.Recv()

		if err != nil {
			if err == io.EOF {
				break
			}

			continue
		}

		payload := response.Payload.GetVisionFrame()
		frame, err := service.ProcessLiveStream(payload)

		for conn := range gc.socketConns {
			go func(conn *websocket.Conn) {
				if _, err := conn.Write(frame); err != nil {
					conn.Close()
				}
			}(conn)
		}
	}

	return nil
}

func (gc *GrpcClient) GetVisionChunk() (domain.Chunk, error) {
	client := pb.NewGatewayServiceClient(gc.conn)

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	response, err := client.GetVisionChunk(ctx, &pb.GetVisionChunkRequest{
		Header: &pb.ChunkRequestHeader{
			Start: &durationpb.Duration{
				Seconds: 0,
			},
		},
	})

	if err != nil {
		return domain.Chunk{}, err
	}

	chunk := domain.Chunk{
		Payloads: response.Payloads,
	}

	return chunk, nil
}

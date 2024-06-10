package client

import (
	"context"
	"fmt"
	"io"
	"log"

	"github.com/robocin/ssl-core/player-bff/internal/application"
	"github.com/robocin/ssl-core/player-bff/internal/entity"
	pb "github.com/robocin/ssl-core/player-bff/pkg/pb/ui"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/protobuf/types/known/durationpb"
)

type GrpcClient struct {
	address string
	conn    *grpc.ClientConn
	client  pb.GatewayServiceClient
}

func NewGrpcClient(address string) *GrpcClient {
	opt := grpc.WithTransportCredentials(insecure.NewCredentials())
	conn, err := grpc.Dial(address, opt)

	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	client := pb.NewGatewayServiceClient(conn)

	return &GrpcClient{
		address: address,
		conn:    conn,
		client:  client,
	}
}

func (gc *GrpcClient) GetConn() interface{} {
	return gc.conn
}

func (gc *GrpcClient) Close() error {
	return gc.conn.Close()
}

func (gc *GrpcClient) ReceiveLiveStream(proxy *application.ConnectionProxy) error {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	req := &pb.ReceiveLiveStreamRequest{
		MessageTypes: []pb.MessageType{
			pb.MessageType_MESSAGE_TYPE_VISION,
		},
	}

	stream, err := gc.client.ReceiveLiveStream(ctx, req)

	if err != nil {
		fmt.Println(err)
		return err
	}

	for {
		response, err := stream.Recv()

		if err != nil {
			if err == io.EOF {
				break
			}

			fmt.Println(err)
			continue
		}

		payload := response.Payload.GetVisionFrame()

		if payload.Properties == nil {
			continue
		}

		frame := entity.NewFrameFromProto(payload)
		event := application.NewConnectionProxyEvent("frame", frame)
		proxy.Notify(event)
	}

	return nil
}

func (gc *GrpcClient) ReceiveReplay(proxy *application.ConnectionProxy) error {
	client := pb.NewGatewayServiceClient(gc.conn)

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	stream, err := client.ReceiveLiveStream(ctx, &pb.ReceiveLiveStreamRequest{
		MessageTypes: []pb.MessageType{
			pb.MessageType_MESSAGE_TYPE_VISION,
		},
	})

	if err != nil {
		fmt.Println(err)
		return err
	}

	for {
		response, err := stream.Recv()

		if err != nil {
			if err == io.EOF {
				break
			}

			fmt.Println(err)
			continue
		}

		payload := response.Payload.GetVisionFrame()

		if payload.Properties == nil {
			continue
		}

		chunk := entity.NewFrameFromProto(payload)
		event := application.NewConnectionProxyEvent("chunk", chunk)
		proxy.Notify(event)
	}

	return nil
}

func (gc *GrpcClient) GetVisionChunk() (entity.Chunk, error) {
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
		return entity.Chunk{}, err
	}

	chunk := entity.Chunk{
		Payloads: response.Payloads,
	}

	return chunk, nil
}

package client

import (
	"context"
	"fmt"
	"io"
	"log"
	"time"

	"github.com/robocin/ssl-core/player-bff/internal/application"
	"github.com/robocin/ssl-core/player-bff/internal/entity"
	pb "github.com/robocin/ssl-core/player-bff/pkg/pb/gateway"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type GrpcClient struct {
	address string
	conn    *grpc.ClientConn
	client  pb.GatewayServiceClient
}

func NewGrpcClient(address string) *GrpcClient {
	opt := grpc.WithTransportCredentials(insecure.NewCredentials())
	conn, err := grpc.NewClient(address, opt)

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
	fmt.Println("ReceiveLiveStream...")
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	stream, err := gc.client.ReceiveLivestream(ctx)

	if err != nil {
		fmt.Println(err)
		return err
	}

	req := &pb.ReceiveLivestreamRequest{}
	stream.Send(req)

	for {
		response, err := stream.Recv()

		if err != nil {
			if err == io.EOF {
				break
			}

			fmt.Println(err)
			continue
		}

		payload := response.Sample

		if payload.String() == "" {
			continue
		}

		sample := entity.NewFrame(payload)
		event := application.NewConnectionProxyEvent("sample", sample)
		proxy.Notify(event)
	}

	return nil
}

func (gc *GrpcClient) GetReplayChunk(timestamp int64) (entity.Chunk, error) {
	client := pb.NewGatewayServiceClient(gc.conn)

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	startTimestamp := timestamppb.New(time.UnixMilli(timestamp))

	response, err := client.GetReplayChunk(ctx, &pb.GetReplayChunkRequest{StartTimestamp: startTimestamp})

	if err != nil {
		return entity.Chunk{}, err
	}

	frames := make([]entity.Frame, 0)

	for _, sample := range response.GetSamples() {
		frames = append(frames, entity.NewFrame(sample))
	}

	chunk := entity.Chunk{
		Frames:  frames,
		EndTime: response.GetLastTimestamp().AsTime(),
	}

	return chunk, nil
}

func (gc *GrpcClient) GetGameEvents() ([]entity.Event, error) {
	client := pb.NewGatewayServiceClient(gc.conn)

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	response, err := client.GetGameEvents(ctx, &pb.GetGameEventsRequest{})

	if err != nil {
		return []entity.Event{}, err
	}

	events := make([]entity.Event, 0)

	for _, event := range response.GetGameEvents() {
		events = append(events, entity.NewEvent(event))
	}

	return events, nil
}

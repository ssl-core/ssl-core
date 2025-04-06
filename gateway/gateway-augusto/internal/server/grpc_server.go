package server

import (
	"context"
	"fmt"
	"net"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/socket"
	"github.com/robocin/ssl-core/protocols/gateway"
	"github.com/robocin/ssl-core/protocols/playback"
	"google.golang.org/grpc"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/reflect/protoreflect"
)

const protocol = "tcp"

type GrpcServer struct {
	server            *grpc.Server
	address           string
	subscriberHandler socket.SocketReceiverHandler
	dealer            network.ZmqDealerSocket

	gateway.UnimplementedGatewayServiceServer
}

func NewGrpcServer(address string) *GrpcServer {
	server := grpc.NewServer()

	return &GrpcServer{
		server:  server,
		address: address,
		//TODO: service discovery usage
		subscriberHandler: *socket.NewSocketReceiverHandler(network.NewZmqSubscriberSocket("ipc:///tmp/.ssl-core/playback.ipc", "sample"), nil),
		dealer:            *network.NewZmqDealerSocket("ipc:///tmp/.ssl-core/replay.ipc"),
	}
}

func NewGrpcServerWithSubscriberHandler(address string, subscriberHandler socket.SocketReceiverHandler) *GrpcServer {
	server := grpc.NewServer()

	return &GrpcServer{
		server:            server,
		address:           address,
		subscriberHandler: subscriberHandler,
		dealer:            *network.NewZmqDealerSocket("ipc:///tmp/.ssl-core/replay.ipc"),
	}
}

func (s *GrpcServer) Start() {
	lis, err := net.Listen(protocol, s.address)

	if err != nil {
		panic(err)
	}

	gateway.RegisterGatewayServiceServer(s.server, s)
	s.server.Serve(lis)
}

func (s *GrpcServer) ReceiveLivestream(stream gateway.GatewayService_ReceiveLivestreamServer) error {
	for {
		// TODO: Handle stream.Recv()
		datagram := s.subscriberHandler.OnReceive()
		var sample playback.Sample
		err := proto.Unmarshal(datagram.Message, &sample)

		// fmt.Println("Sending sample: ", sample)

		if err != nil {
			fmt.Println("Error unmarshaling sample: ", datagram.Message, sample)
			continue
		}

		response := &gateway.ReceiveLivestreamResponse{
			Sample: &sample,
		}

		if err := stream.Send(response); err != nil {
			return err
		}
	}
}

func (s *GrpcServer) GetReplayChunk(ctx context.Context, request *gateway.GetReplayChunkRequest) (*gateway.GetReplayChunkResponse, error) {
	data, err := proto.Marshal(request)

	if err != nil {
		return &gateway.GetReplayChunkResponse{}, err
	}
	var response gateway.GetReplayChunkResponse
	err = s.handleUnaryRequest(&response, data)
	return &response, err
}

func (s *GrpcServer) GetGameStatusEvent(ctx context.Context, request *gateway.GetGameEventsRequest) (*gateway.GetGameEventsResponse, error) {
	data, err := proto.Marshal(request)

	if err != nil {
		return &gateway.GetGameEventsResponse{}, err
	}

	var response gateway.GetGameEventsResponse
	err = s.handleUnaryRequest(&response, data)
	return &response, err
}

func (s *GrpcServer) handleUnaryRequest(response protoreflect.ProtoMessage, request []byte) error {
	s.dealer.Send(*network.NewZmqDatagram(request))
	datagram := s.dealer.Receive()

	return proto.Unmarshal(datagram.Message, response)
}

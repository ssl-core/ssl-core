package server

import (
	"io"
	"net"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/pkg/pb/gateway"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/pkg/pb/playback"
	"google.golang.org/grpc"
	"google.golang.org/protobuf/proto"
)

const protocol = "tcp"

type GrpcServer struct {
	server     *grpc.Server
	address    string
	subscriber network.ZmqSubscriberSocket

	gateway.UnimplementedGatewayServiceServer
}

func NewGrpcServer(address string) *GrpcServer {
	server := grpc.NewServer()

	return &GrpcServer{
		server:  server,
		address: address,
		//TODO: service discovery usage
		subscriber: *network.NewZmqSubscriberSocket("ipc:///tmp/playback.ipc", ""),
	}
}

func (s *GrpcServer) Start() {
	gateway.RegisterGatewayServiceServer(s.server, s)
	lis, err := net.Listen(protocol, s.address)

	if err != nil {
		panic(err)
	}

	s.server.Serve(lis)
}

func (s *GrpcServer) ReceiveLivestream(stream gateway.GatewayService_ReceiveLivestreamServer) error {
	for {
		_, err := stream.Recv()

		if err == io.EOF || err != nil {
			continue
		}

		datagram := s.subscriber.Receive()
		var sample playback.Sample
		err = proto.Unmarshal(datagram.Message, &sample)
		response := &gateway.ReceiveLivestreamResponse{
			Sample: &sample,
		}

		if err := stream.Send(response); err != nil {
			return err
		}
	}
}

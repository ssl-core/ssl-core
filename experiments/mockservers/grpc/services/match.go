package services

import (
	"context"
	pb "mockserver/grpc/pb"
	"time"
)

type MatchService struct {
	pb.UnimplementedMatchServiceServer
}

func (ms MatchService) Get(ctx context.Context, req *pb.MatchRequest) (*pb.MatchResponse, error) {
	return &pb.MatchResponse{
		Match: &pb.Match{
			Id:   "1",
			Name: "Arsenal",
		},
	}, nil
}

func (ms MatchService) Live(req *pb.MatchRequest, stream pb.MatchService_LiveServer) error {
	for i := 0; i < 10; i++ {
		response := &pb.MatchResponse{
			Match: &pb.Match{
				Id:   "1",
				Name: "Arsenal",
			},
		}

		if err := stream.Send(response); err != nil {
			return err
		}

		// sleep
		time.Sleep(1 * time.Second)
	}

	return nil
}

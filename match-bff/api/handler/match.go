package handler

import (
	"context"

	pb "github.com/robocin/ssl-core/match-bff/pkg/pb"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
)

type MatchHandler struct {
	pb.UnimplementedMatchServiceServer
}

func (h MatchHandler) Get(ctx context.Context, req *pb.MatchRequest) (res *pb.MatchResponse, err error) {
	if req.Req == "" {
		err = status.New(codes.InvalidArgument, "Field cannot be empty").Err()
		return
	}

	Match := &pb.Match{
		Id:   "1",
		Name: "Match",
	}

	response := &pb.MatchResponse{
		Match: Match,
	}

	return response, nil
}

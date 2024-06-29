package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/gateway"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Chunk struct {
	EndTime time.Time `json:"end_time"`
	Samples []Sample  `json:"samples"`
}

func NewChunk(endTime time.Time, samples []Sample) *Chunk {
	return &Chunk{
		EndTime: endTime,
		Samples: samples,
	}
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

func (c *Chunk) ToProto(last_timestamp time.Time) (*gateway.GetReplayChunkResponse, error) {
	samples_pb := make([]*playback.Sample, len(c.Samples))
	for i, sample := range c.Samples {
		samples_pb[i] = sample.ToProto()
	}

	return &gateway.GetReplayChunkResponse{
		LastTimestamp: timestamppb.New(last_timestamp),
		Samples:       samples_pb,
	}, nil
}

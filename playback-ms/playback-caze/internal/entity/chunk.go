package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/gateway"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Chunk struct {
	LatestTimestamp time.Time `json:"latest_timestamp"`
	Samples         []Sample  `json:"samples"`
}

func NewChunk(latestTime time.Time, samples []Sample) *Chunk {
	return &Chunk{
		LatestTimestamp: latestTime,
		Samples:         samples,
	}
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

func (c *Chunk) ToProto() *gateway.GetReplayChunkResponse {
	samples_pb := make([]*playback.Sample, len(c.Samples))
	for i, sample := range c.Samples {
		samples_pb[i] = sample.ToProto()
	}

	return &gateway.GetReplayChunkResponse{
		LastTimestamp: timestamppb.New(c.LatestTimestamp),
		Samples:       samples_pb,
	}
}

package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Sample struct {
	FirstTimestamp time.Time `json:"first_timestamp"`
	Timestamp      time.Time `json:"timestamp"`
	Detection      Detection `json:"detection"`
}

func NewSample(sample *playback.Sample) Sample {
	return Sample{
		Detection: NewDetection(sample.Detection),
	}
}

func (s *Sample) ToJson() ([]byte, error) {
	return json.Marshal(s)
}

func (s *Sample) ToProto() *playback.Sample {
	first_timestamp := timestamppb.New(s.FirstTimestamp)
	timestamp := timestamppb.New(s.Timestamp)
	detection := s.Detection.ToProto()

	return &playback.Sample{
		FirstTimestamp: first_timestamp,
		Timestamp:      timestamp,
		Detection:      detection,
	}
}

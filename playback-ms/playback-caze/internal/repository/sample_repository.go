package repository

import (
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type ISampleRepository interface {
	AddSample(sample *playback.Sample) error
	AddSamples(samples []*playback.Sample) error
	GetLatestSample() (*playback.Sample, error)
	GetSamples(*timestamppb.Timestamp, *timestamppb.Timestamp) ([]*playback.Sample, error)
}

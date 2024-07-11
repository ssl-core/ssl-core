package repository

import (
	"fmt"

	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type SampleRepository interface {
	AddSample(sample *playback.Sample) error
	AddSamples(samples []*playback.Sample) error
	GetSamples(*timestamppb.Timestamp, *timestamppb.Timestamp) ([]*playback.Sample, error)
}

func makeRedisRepository() SampleRepository {
	return &sampleRedisRepository{
		redisClient: redis_db.NewRedisClient(sampleStream),
	}
}

func NewSampleRepository(repositoryType string) SampleRepository {
	if repositoryType == "redis" {
		return makeRedisRepository()
	}
	panic(fmt.Errorf("invalid repository type %s", repositoryType))
}

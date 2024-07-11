package repository

import (
	"fmt"
	"log"

	"github.com/redis/go-redis/v9"
	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type SampleRedisRepository struct {
	redisClient *redis_db.RedisClient
}

func (r *SampleRedisRepository) AddSample(sample *playback.Sample) error {
	return r.redisClient.Set(sample.Timestamp.AsTime(), sample.String())
}

func (r *SampleRedisRepository) AddSamples(samples []*playback.Sample) error {
	for _, sample := range samples {
		err := r.AddSample(sample)
		if err != nil {
			return err
		}
	}
	return nil
}

func (r *SampleRedisRepository) GetLatestSample() (*playback.Sample, error) {
	dbResult, err := r.redisClient.GetLatest()
	if err != nil {
		return nil, err
	}
	samples := r.samplesMapper(dbResult)
	if len(samples) == 1 {
		return samples[0], nil
	}
	return nil, fmt.Errorf("error getting latest sample")
}

func (r *SampleRedisRepository) GetSamples(startTime, endTime *timestamppb.Timestamp) ([]*playback.Sample, error) {
	chunkResult, err := r.redisClient.GetChunk(startTime.AsTime(), endTime.AsTime())
	if err != nil {
		return nil, err
	}
	return r.samplesMapper(chunkResult), nil
}

// HELPERS:

func (r *SampleRedisRepository) sampleMapper(values map[string]interface{}) (*playback.Sample, error) {
	if value, ok := values["value"]; ok {
		var sample playback.Sample
		err := proto.Unmarshal([]byte(value.(string)), &sample)
		if err != nil {
			return nil, err
		}
		return &sample, nil
	}
	return nil, fmt.Errorf("error mapping sample from redis values")
}

func (r *SampleRedisRepository) samplesMapper(dbResult interface{}) []*playback.Sample {
	samples := make([]*playback.Sample, 0)
	result, ok := dbResult.(redis.XMessageSliceCmd)
	if !ok {
		fmt.Println("Error casting dbResult to redis.XMessageSliceCmd")
		return samples
	}
	for _, message := range result.Val() {
		sample, err := r.sampleMapper(message.Values)
		if err != nil {
			log.Printf("Error mapping sample: %v", err)
			continue
		}
		samples = append(samples, sample)
	}
	return samples
}

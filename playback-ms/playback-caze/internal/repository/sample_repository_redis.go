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

const (
	sampleStream = "sample"
)

type sampleRedisRepository struct {
	redisClient *redis_db.RedisClient
}

func (r *sampleRedisRepository) AddSample(sample *playback.Sample) error {
	return r.redisClient.Set(sample.Timestamp.AsTime(), sample.String())
}

func (r *sampleRedisRepository) AddSamples(samples []*playback.Sample) error {
	for _, sample := range samples {
		err := r.AddSample(sample)
		if err != nil {
			return err
		}
	}
	return nil
}

func (r *sampleRedisRepository) GetSamples(startTime, endTime *timestamppb.Timestamp) ([]*playback.Sample, error) {
	chunkResult, err := r.redisClient.GetChunk(startTime.AsTime(), endTime.AsTime())
	if err != nil {
		return nil, err
	}
	return samplesMapper(chunkResult), nil
}

// HELPERS:

func sampleMapper(values map[string]interface{}) (*playback.Sample, error) {
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

func samplesMapper(dbResult interface{}) []*playback.Sample {
	samples := make([]*playback.Sample, 0)
	result, ok := dbResult.(redis.XMessageSliceCmd)
	if !ok {
		fmt.Println("Error casting dbResult to redis.XMessageSliceCmd")
		return samples
	}
	for _, message := range result.Val() {
		sample, err := sampleMapper(message.Values)
		if err != nil {
			log.Printf("Error mapping sample: %v", err)
			continue
		}
		samples = append(samples, sample)
	}
	return samples
}

package repository

import (
	"fmt"
	"log"

	"github.com/redis/go-redis/v9"
	"github.com/robocin/ssl-core/common/golang/time_util"
	redis_db "github.com/robocin/ssl-core/playback-ms/playback-caze/db/redis"
	"github.com/robocin/ssl-core/protocols/playback"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type SampleRedisRepository struct {
	redisClient *redis_db.RedisClient
}

func (r *SampleRedisRepository) AddSample(sample *playback.Sample) error {
	message, err := proto.Marshal(sample)

	if err != nil {
		return err
	}

	return r.redisClient.Set(time_util.TimeFromTimestamp(sample.GetTimestamp()), message)
}

func (r *SampleRedisRepository) AddSamples(samples []*playback.Sample) {
	for _, sample := range samples {
		err := r.AddSample(sample)

		if err != nil {
			fmt.Println("Failed to save sample:", err)
		}
	}
}

func (r *SampleRedisRepository) GetLatestSample() (*playback.Sample, error) {
	dbResult, err := r.redisClient.GetLatest()
	if err != nil {
		return nil, err
	}
	samples := samplesMapper(dbResult)
	if len(samples) == 1 {
		return samples[0], nil
	}
	return nil, fmt.Errorf("error getting latest sample")
}

func (r *SampleRedisRepository) GetSamples(startTime *timestamppb.Timestamp, endTime *timestamppb.Timestamp) ([]*playback.Sample, error) {
	chunkResult, err := r.redisClient.GetChunk(time_util.TimeFromTimestamp(startTime), time_util.TimeFromTimestamp(endTime))
	if err != nil {
		return nil, err
	}
	return samplesMapper(chunkResult), nil
}

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
	result, ok := dbResult.([]redis.XMessage)
	if !ok {
		fmt.Println("error casting dbResult to []redis.XMessage")
		return samples
	}
	for _, message := range result {
		sample, err := sampleMapper(message.Values)
		if err != nil {
			log.Printf("error mapping sample: %v", err)
			continue
		}
		samples = append(samples, sample)
	}
	return samples
}

package repository

import (
	"fmt"
	"log"

	"github.com/redis/go-redis/v9"
	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/internal/time_util"
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
	message, err := proto.Marshal(sample)

	if err != nil {
		return err
	}

	return r.redisClient.Set(time_util.TimeFromTimestamp(sample.GetTimestamp()), message)
}

func (r *sampleRedisRepository) AddSamples(samples []*playback.Sample) {
	for _, sample := range samples {
		err := r.AddSample(sample)

		if err != nil {
			fmt.Println("Failed to save sample:", err)
		}
	}
}

func (r *sampleRedisRepository) GetSamples(startTime *timestamppb.Timestamp, endTime *timestamppb.Timestamp) ([]*playback.Sample, error) {
	chunkResult, err := r.redisClient.GetChunk(time_util.TimeFromTimestamp(startTime), time_util.TimeFromTimestamp(endTime))
	if err != nil {
		return nil, err
	}
	return samplesMapper(chunkResult), nil
}

func sampleMapper(values map[string]interface{}) (*playback.Sample, error) {
	if encoded, ok := values["value"]; ok {
		var sample playback.Sample
		err := proto.Unmarshal(encoded.([]byte), &sample)
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
		fmt.Println("error casting dbResult to redis.XMessageSliceCmd")
		return samples
	}
	for _, message := range result.Val() {
		sample, err := sampleMapper(message.Values)
		if err != nil {
			log.Printf("error mapping sample: %v", err)
			continue
		}
		samples = append(samples, sample)
	}
	return samples
}

package redis_db

import (
	"context"
	"fmt"
	"time"

	"github.com/redis/go-redis/v9"
)

const (
	redisAddress = "redis"
	redisPort    = 6379
)

type RedisClient struct {
	client *redis.Client
	stream string
}

func NewRedisClient(stream string) *RedisClient {
	options := redis.Options{
		Addr:     fmt.Sprintf("%s:%d", redisAddress, redisPort),
		Password: "",
		DB:       0,
	}
	fmt.Printf("Creating new RedisClient with options %v...\n", options)
	client := redis.NewClient(&options)

	return &RedisClient{
		client: client,
		stream: stream,
	}
}

func (rc *RedisClient) Close() {
	rc.client.Close()
}

func (rc *RedisClient) Set(key time.Time, value interface{}) error {
	ctx := context.Background()
	args := &redis.XAddArgs{
		Stream: rc.stream,
		ID:     fmt.Sprintf("%d", key.UnixMilli()),
		Values: map[string]interface{}{
			"value": value,
		},
	}
	return rc.client.XAdd(ctx, args).Err()
}

func (rc *RedisClient) Get(key time.Time) (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRangeN(ctx, rc.stream, fmt.Sprintf("%d", key.UnixMilli()), "+", 1).Result()
}

func (rc *RedisClient) GetChunk(start_time, end_time time.Time) interface{} {
	ctx := context.Background()
	return rc.client.XRange(ctx, rc.stream, fmt.Sprintf("%d", start_time.UnixMilli()), fmt.Sprintf("%d", end_time.UnixMilli())).Val()
}

func (rc *RedisClient) Clear() error {
	ctx := context.Background()
	return rc.client.FlushAll(ctx).Err()
}

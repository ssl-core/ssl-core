package redis_db

import (
	"context"
	"fmt"
	"time"

	"github.com/redis/go-redis/v9"
)

const (
	redisAddress = "redis-db"
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
		ID:     rc.convertTimeToKey(key),
		Values: map[string]interface{}{
			"value": value,
		},
	}

	return rc.client.XAdd(ctx, args).Err()
}

func (rc *RedisClient) GetLatest() (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRevRangeN(ctx, rc.stream, "+", "-", 1).Result()
}

func (rc *RedisClient) Get(key time.Time) (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRangeN(ctx, rc.stream, rc.convertTimeToKey(key), "+", 1).Result()
}

func (rc *RedisClient) GetChunk(start_time, end_time time.Time) (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRange(ctx, rc.stream, rc.convertTimeToKey(start_time), rc.convertTimeToKey(end_time)).Result()
}

func (rc *RedisClient) Clear() error {
	ctx := context.Background()
	return rc.client.FlushAll(ctx).Err()
}

func (rc *RedisClient) convertTimeToKey(t time.Time) string {
	return fmt.Sprintf("%d", t.UnixMilli())
}

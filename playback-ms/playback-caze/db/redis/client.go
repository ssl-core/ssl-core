package redis_db

import (
	"context"
	"fmt"
	"strconv"
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
		ID:     timeToKey(key),
		Values: map[string]interface{}{
			"value": value,
		},
	}
	// fmt.Printf("Setting key %s for time %v...\n", timeToKey(key), key)
	return rc.client.XAdd(ctx, args).Err()
}

func (rc *RedisClient) GetLatest() (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRevRangeN(ctx, rc.stream, "+", "-", 1).Result()
}

func (rc *RedisClient) Get(key time.Time) (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRangeN(ctx, rc.stream, timeToKey(key), "+", 1).Result()
}

func (rc *RedisClient) GetChunk(startTime time.Time, endTime time.Time) (interface{}, error) {
	ctx := context.Background()
	return rc.client.XRange(ctx, rc.stream, timeToKey(startTime), timeToKey(endTime)).Result()
}

func (rc *RedisClient) Clear() error {
	ctx := context.Background()
	return rc.client.FlushAll(ctx).Err()
}

func timeToKey(t time.Time) string {
	return strconv.FormatInt(t.UnixNano(), 10)
}

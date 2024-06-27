package redis

import (
	"context"

	"github.com/redis/go-redis/v9"
)

type RedisClient struct {
	client *redis.Client
}

func NewRedisClient() *RedisClient {
	client := redis.NewClient(&redis.Options{
		Addr:     "172.17.0.4:6379",
		Password: "",
		DB:       0,
	})

	return &RedisClient{
		client: client,
	}
}

func (rc *RedisClient) Get(key string) (interface{}, error) {
	ctx := context.Background()
	return rc.client.Get(ctx, key).Result()
}

func (rc *RedisClient) Set(key string, value interface{}) error {
	ctx := context.Background()
	return rc.client.Set(ctx, key, value, 0).Err()
}

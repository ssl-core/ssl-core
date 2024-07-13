package repository

import (
	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
)

const (
	sampleStream     = "sample"
	gameEventsStream = "events"
)

type RedisFactory struct {
}

func (rf *RedisFactory) MakeSampleRepository() ISampleRepository {
	return &SampleRedisRepository{
		redisClient: redis_db.NewRedisClient(sampleStream),
	}
}

func (a *RedisFactory) MakeGameEventsRepository() IGameEventsRepository {
	return &GameEventsRedisRepository{
		redisClient: redis_db.NewRedisClient(gameEventsStream),
	}
}

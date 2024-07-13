package repository

import (
	"fmt"
	"log"

	"github.com/redis/go-redis/v9"
	redis_db "github.com/robocin/ssl-core/playback-ms/db/redis"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type GameEventsRedisRepository struct {
	redisClient *redis_db.RedisClient
}

func (r *GameEventsRedisRepository) AddGameEvents(timestamp *timestamppb.Timestamp, gameEvents []*common.GameEvent) error {
	return r.redisClient.Set(timestamp.AsTime(), gameEvents)
}

func (r *GameEventsRedisRepository) GetGameEventsFrom(timestamp *timestamppb.Timestamp) ([]*common.GameEvent, error) {
	chunkResult, err := r.redisClient.GetChunk(timestamp.AsTime(), timestamp.AsTime())
	if err != nil {
		return nil, err
	}
	return r.gameEventsMapper(chunkResult), nil
}

func (r *GameEventsRedisRepository) gameEventMapper(values map[string]interface{}) (*common.GameEvent, error) {
	if value, ok := values["value"]; ok {
		var event common.GameEvent
		err := proto.Unmarshal([]byte(value.(string)), &event)
		if err != nil {
			return nil, err
		}
		return &event, nil
	}
	return nil, fmt.Errorf("error mapping event from redis values")
}

func (r *GameEventsRedisRepository) gameEventsMapper(dbResult interface{}) []*common.GameEvent {
	events := make([]*common.GameEvent, 0)
	result, ok := dbResult.(redis.XMessageSliceCmd)
	if !ok {
		fmt.Println("Error casting dbResult to redis.XMessageSliceCmd")
		return events
	}
	for _, message := range result.Val() {
		event, err := r.gameEventMapper(message.Values)
		if err != nil {
			log.Printf("Error mapping event: %v", err)
			continue
		}
		events = append(events, event)
	}
	return events
}

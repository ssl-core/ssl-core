package main

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
)

func redis_experiment() {
	redis_client := redis.NewRedisClient()

	start := 1
	end := 10

	for i := start; i <= end; i++ {
		redis_client.Set(fmt.Sprintf("key_%d", i), fmt.Sprintf("value = %d", i))
	}

	for i := start; i <= end; i++ {
		value, err := redis_client.Get(fmt.Sprintf("key_%d", i))
		if err != nil {
			fmt.Printf("Error getting value for key_%d: %v\n", i, err)
		}
		fmt.Printf("key_%d: %s\n", i, value)
	}
}

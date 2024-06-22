package main

import (
	"fmt"

	"github.com/robocin/ssl-core/experiments/db_go/infra/redis"
)

func main() {
	client := redis.NewRedisClient()
	err := client.Set("teste", "oid")

	if err != nil {
		fmt.Println("Err: ", err)
	}

	result, err := client.Get("teste")

	if err != nil {
		fmt.Println("Error: ", err)
		return
	}

	fmt.Println(result)
}

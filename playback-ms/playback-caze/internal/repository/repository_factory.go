package repository

import (
	"fmt"
)

type IRepositoryFactory interface {
	MakeSampleRepository() ISampleRepository
	MakeGameEventsRepository() IGameEventsRepository
}

func GetRepositoryFactory(db string) IRepositoryFactory {
	if db == "redis" {
		return &RedisFactory{}
	}

	panic(fmt.Errorf("invalid repository type %s", db))
}

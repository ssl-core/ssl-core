package db

import (
	"encoding/json"
	"fmt"
	"time"

	"github.com/robocin/ssl-core/playback-ms/db/redis"
)

const (
	ChunkStream = "test"
)

type DatabaseExperiment struct {
	client DBClient
}

func NewDatabaseExperiment() *DatabaseExperiment {
	return &DatabaseExperiment{
		client: redis.NewRedisClient(ChunkStream),
	}
}

func (de *DatabaseExperiment) databaseExperimentSetGet() {
	fmt.Printf("Starting databaseExperimentSetGet...\n")
	defer fmt.Printf("------------------------------\n")

	start := 1
	end := 10

	for i := start; i <= end; i++ {
		de.client.Set(timeAddingSeconds(i), fmt.Sprintf("value_%d", i))
	}

	for i := start; i <= end; i++ {
		key := timeAddingSeconds(i)
		result, err := de.client.Get(key)
		if err != nil {
			fmt.Printf("Error getting value for KEY(%d): %v\n", i, err)
			continue
		}

		values, err := json.Marshal(result)
		if err != nil {
			fmt.Printf("Error marshalling value for KEY(%d): %v\n", i, err)
			continue
		}
		fmt.Printf("KEY(%d) == %s\n", i, string(values))
	}
}

func (de *DatabaseExperiment) databaseExperimentSetGetRange() {
	fmt.Printf("Starting databaseExperimentSetGetRange...\n")
	defer fmt.Printf("------------------------------\n")

	start := 20
	end := 1000
	for i := start; i <= end; i++ {
		de.client.Set(timeAddingSeconds(i), fmt.Sprintf("value = %d", i))
	}

	start_second := 50
	end_second := 100
	start_time := timeAddingSeconds(start_second)
	end_time := timeAddingSeconds(end_second)
	values, err := de.client.GetChunk(start_time, end_time)
	if err != nil {
		fmt.Printf("Error getting chunk from %d to %d: %v\n", start, end, err)
	}
	fmt.Printf("Chunk from %ds to %ds: %v\n", start_second, end_second, values)
}

func (de *DatabaseExperiment) clearDatabase() {
	de.client.Clear()
}

func timeAddingSeconds(seconds int) time.Time {
	return time.Date(2024, time.February, 1, 0, 0, 0, 0, time.UTC).Add(time.Duration(seconds) * time.Second)
}

func (de *DatabaseExperiment) Run() {
	de.clearDatabase()
	de.databaseExperimentSetGet()
	de.databaseExperimentSetGetRange()
}

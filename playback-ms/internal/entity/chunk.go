package entity

import (
	"encoding/json"
	"time"
)

type Chunk struct {
	EndTime time.Time `json:"end_time"`
	Samples []Sample  `json:"samples"`
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

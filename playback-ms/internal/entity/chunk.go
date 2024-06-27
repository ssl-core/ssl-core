package entity

import (
	"encoding/json"
	"time"
)

type Chunk struct {
	EndTime    time.Time   `json:"end_time"`
	Detections []Detection `json:"detections"`
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

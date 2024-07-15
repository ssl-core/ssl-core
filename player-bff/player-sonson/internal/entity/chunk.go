package entity

import (
	"encoding/json"
	"time"
)

type Chunk struct {
	RequestTime time.Time `json:"request_time"`
	EndTime     time.Time `json:"end_time"`
	Frames      []Frame   `json:"frames"`
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

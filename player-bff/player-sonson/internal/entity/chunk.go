package entity

import (
	"encoding/json"
	"time"
)

type Chunk struct {
	EndTime time.Time `json:"end_time"`
	Frames  []Frame   `json:"frames"`
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

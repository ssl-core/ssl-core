package entity

import (
	"encoding/json"
	"time"
)

type Chunk struct {
	EndTime time.Time `json:"end_time"`
	Samples []Sample  `json:"samples"`
}

func NewChunk(endTime time.Time, samples []Sample) *Chunk {
	return &Chunk{
		EndTime: endTime,
		Samples: samples,
	}
}

func (c *Chunk) ToJson() ([]byte, error) {
	return json.Marshal(c)
}

// TODO(matheusvtna): Implement Chunk.ToProto() method. Needs the playback.Chunk protobuf.
// func (c *Chunk) ToProto() *playback.Chunk {
// 	samples_pb := make([]*playback.Sample, len(c.Samples))
// 	for i, sample := range c.Samples {
// 		samples_pb[i] = sample.ToProto()
// 	}

// 	return &playback.Chunk{
// 		...
// 		samples: samsamples_pb
// 	}
// }

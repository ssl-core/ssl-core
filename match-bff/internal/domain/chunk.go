package domain

import "encoding/json"

type Chunk struct {
	Payloads []string
}

func (c *Chunk) ToJson() ([]byte, error) {
	data := map[string]interface{}{
		"payloads": c.Payloads,
	}

	return json.Marshal(data)
}

package service

import (
	"encoding/json"

	"github.com/robocin/ssl-core/match-bff/internal/entity"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

func ProcessLiveStream(visionFrame *vision.Frame) ([]byte, error) {
	frame := entity.NewFrameFromProto(visionFrame)

	response := map[string]interface{}{
		"type":    "frame",
		"payload": frame,
	}

	return json.Marshal(response)
}

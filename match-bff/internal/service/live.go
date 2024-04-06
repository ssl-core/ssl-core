package service

import (
	"github.com/robocin/ssl-core/match-bff/internal/entity"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

func ProcessLiveStream(visionFrame *vision.Frame) ([]byte, error) {
	frame := entity.NewFrameFromProto(visionFrame)
	return frame.ToJson()
}

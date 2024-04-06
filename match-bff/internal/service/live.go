package service

import (
	"github.com/robocin/ssl-core/match-bff/internal/domain"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

func ProcessLiveStream(visionFrame *vision.Frame) ([]byte, error) {
	frame := domain.NewFrameFromProto(visionFrame)
	return frame.ToJson()
}

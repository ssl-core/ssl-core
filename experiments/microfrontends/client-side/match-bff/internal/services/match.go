package services

import (
	"robocin-ines/match-bff/internal/domain"
)

type MatchService struct {
}

func NewMatchService() *MatchService {
	return &MatchService{}
}

func (s *MatchService) GetMatchChunk(id string, timestamp int64) []domain.Frame {
	frames := []domain.Frame{}

	return frames
}

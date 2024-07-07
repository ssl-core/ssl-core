package entity

import (
	"encoding/json"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Sample struct {
	FirstTimestamp   time.Time        `json:"first_timestamp"`
	Timestamp        time.Time        `json:"timestamp"`
	Field            Field            `json:"field"`
	Detection        Detection        `json:"detection"`
	RawDetection     RawDetection     `json:"raw_detection"`
	TrackedDetection TrackedDetection `json:"tracked_detection"`
	GameStatus       GameStatus       `json:"game_status"`
}

// func NewSample(sample *playback.Sample) Sample {
// 	return Sample{
// 		Detection: NewDetection(sample.Detection),
// 	}
// }

func (s *Sample) ToJson() ([]byte, error) {
	return json.Marshal(s)
}

func (s *Sample) ToProto() *playback.Sample {
	first_timestamp := timestamppb.New(s.FirstTimestamp)
	timestamp := timestamppb.New(s.Timestamp)
	field := s.Field.ToProto()
	detection := s.Detection.ToProto()
	rawDetection := s.RawDetection.ToProto()
	trackedDetection := s.TrackedDetection.ToProto()
	// gameStatus := s.GameStatus.ToProto()

	return &playback.Sample{
		FirstTimestamp:   first_timestamp,
		Timestamp:        timestamp,
		Field:            field,
		Detection:        detection,
		RawDetection:     rawDetection,
		TrackedDetection: trackedDetection,
	}
}

func (s *Sample) UpdateFromPerceptionDetectionWrapper(detectionWrapperProto *perception.DetectionWrapper) {
	if s.FirstTimestamp.IsZero() {
		s.FirstTimestamp = detectionWrapperProto.Detection.CreatedAt.AsTime()
	}
	s.Timestamp = detectionWrapperProto.Detection.CreatedAt.AsTime()
	s.Detection = *NewDetection(detectionWrapperProto.Detection)

	if rawDetections := detectionWrapperProto.RawDetections; rawDetections != nil {
		s.RawDetection = *NewRawDetectionFromRawPackets(rawDetections)
	}
	if trackedDetections := detectionWrapperProto.TrackedDetections; trackedDetections != nil {
		s.TrackedDetection = *NewTrackedDetectionFromTrackedPackets(trackedDetections)
	}
}

func (s *Sample) UpdateFromRefereeGameStatus(gameStatusProto *referee.GameStatus) {
	s.GameStatus = *NewGameStatusFromRefereeGameStatus(gameStatusProto)
}

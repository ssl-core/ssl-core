package entity

import (
	"encoding/json"
	"fmt"
	"time"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/transport"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type Sample struct {
	FirstTimestamp time.Time `json:"first_timestamp"`
	Timestamp      time.Time `json:"timestamp"`
	Detection      Detection `json:"detection"`
}

func NewSample(sample *playback.Sample) Sample {
	return Sample{
		Detection: NewDetection(sample.Detection),
	}
}

func (s *Sample) UpdateFromDatagram(datagram transport.ZmqMultipartDatagram) {
	s.updateProperty(datagram.Identifier, datagram.Message)
}

func (s *Sample) updateProperty(identifier []byte, message []byte) {
	var topic string = string(identifier)
	switch topic {
	case "detection":
		var detectionProto playback.Detection
		if err := proto.Unmarshal(message, &detectionProto); err != nil {
			fmt.Errorf("failed to unmarshal detection: %v", err)
		}
		s.Detection = NewDetection(&detectionProto)
	default:
		fmt.Errorf("unexpected topic for ZmqMultipartDatagram: %s", topic)
	}
}

func (s *Sample) ToJson() ([]byte, error) {
	return json.Marshal(s)
}

func (s *Sample) ToProto() *playback.Sample {
	first_timestamp := timestamppb.New(s.FirstTimestamp)
	timestamp := timestamppb.New(s.Timestamp)
	detection := s.Detection.ToProto()

	return &playback.Sample{
		FirstTimestamp: first_timestamp,
		Timestamp:      timestamp,
		Detection:      detection,
	}
}

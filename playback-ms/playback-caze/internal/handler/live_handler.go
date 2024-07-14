package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/mappers"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/playback-caze/pkg/pb/referee"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type LiveHandler struct {
	firstTimestamp *timestamppb.Timestamp
	lastGameStatus *playback.GameStatus
}

func NewLiveHandler() *LiveHandler {
	return &LiveHandler{
		firstTimestamp: nil,
		lastGameStatus: nil,
	}
}

func (lh *LiveHandler) Process(datagram *network.ZmqMultipartDatagram) (*playback.Sample, error) {
	topic := string(datagram.Identifier)

	if topic == service_discovery.RefereeGameStatusTopic {
		refereeGameStatus := referee.GameStatus{}
		proto.Unmarshal(datagram.Message, &refereeGameStatus)

		lh.lastGameStatus = mappers.GameStatusMapper(&refereeGameStatus)

		return nil, fmt.Errorf("updating local referee")
	}

	if lh.lastGameStatus == nil {
		return nil, fmt.Errorf("local referee is nil")
	}

	if topic != service_discovery.PerceptionDetectionWrapperTopic {
		return nil, fmt.Errorf("datagram with topic '%s' not processed", topic)
	}

	perceptionDetectionWrapper := perception.DetectionWrapper{}
	proto.Unmarshal(datagram.Message, &perceptionDetectionWrapper)
	detection := perceptionDetectionWrapper.GetDetection()
	// rawDetections := perceptionDetectionWrapper.GetRawDetections()
	// trackedDetections := perceptionDetectionWrapper.GetTrackedDetections()

	if lh.firstTimestamp == nil {
		lh.firstTimestamp = detection.GetCreatedAt()
	}

	sample := playback.Sample{
		FirstTimestamp:   lh.firstTimestamp,
		Timestamp:        detection.GetCreatedAt(),
		Detection:        mappers.DetectionMapper(detection),
		Field:            mappers.FieldMapper(detection.GetField()),
		GameStatus:       lh.lastGameStatus,
		RawDetection:     nil, // TODO(matheusvtna, joseviccruz): fill raw detection
		TrackedDetection: nil, // TODO(matheusvtna, joseviccruz): fill tracked detection
	}

	return &sample, nil
}

package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/internal/mappers"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
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

	sample := playback.Sample{}
	sample.GameStatus = lh.lastGameStatus

	if topic == service_discovery.PerceptionDetectionWrapperTopic {
		perceptionDetectionWrapper := perception.DetectionWrapper{}
		proto.Unmarshal(datagram.Message, &perceptionDetectionWrapper)

		if lh.firstTimestamp == nil {
			lh.firstTimestamp = perceptionDetectionWrapper.GetDetection().GetCreatedAt()
		}
		sample.Detection = mappers.DetectionMapper(perceptionDetectionWrapper.GetDetection())
		sample.Timestamp = sample.Detection.GetCreatedAt()
		sample.FirstTimestamp = lh.firstTimestamp
	} else {
		return nil, fmt.Errorf("datagram with topic '%s' not processed", topic)
	}

	// Use the lines below to debug as json:

	// if json, err := protojson.Marshal(&sample); err == nil {
	// 	fmt.Println("sample as json:", string(json))
	// } else {
	// 	fmt.Println("error parsing Sample to json:", err)
	// }

	return &sample, nil
}

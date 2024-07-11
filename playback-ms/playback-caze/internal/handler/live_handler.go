package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/internal/concurrency"
	"github.com/robocin/ssl-core/playback-ms/internal/mappers"
	"github.com/robocin/ssl-core/playback-ms/internal/repository"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type LiveHandler struct {
	firstTimestamp   *timestamppb.Timestamp
	lastGameStatus   *playback.GameStatus
	samples          *concurrency.ConcurrentQueue[playback.Sample]
	sampleRepository repository.SampleRepository
}

func NewLiveHandler(sampleRepository repository.SampleRepository) *LiveHandler {
	return &LiveHandler{
		firstTimestamp:   nil,
		lastGameStatus:   nil,
		samples:          concurrency.NewQueue[playback.Sample](),
		sampleRepository: sampleRepository,
	}
}

func (lh *LiveHandler) saveSamples() {
	for {
		// samples := lh.samples.DequeueAll()
		// fmt.Printf("Samples: %v\n", samples)
		// err := lh.sampleRepository.AddSamples(samples)
		// if err != nil {
		// 	fmt.Println("Failed to save samples: ", err)
		// }
		for _, sample := range lh.samples.DequeueAll() {
			fmt.Printf("Sample: %v\n", sample)
			err := lh.sampleRepository.AddSample(&sample)
			if err != nil {
				fmt.Println("Failed to save sample")
			}
		}
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

	// enqueue a copy that will be saved into database in another goroutine.
	lh.samples.Enqueue(*proto.Clone(&sample).(*playback.Sample))
	go lh.saveSamples()

	return &sample, nil
}

package handler

import (
	"fmt"
	"time"

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
	firstTimestamp *timestamppb.Timestamp
	lastGameStatus *playback.GameStatus
	times          int64
	start          time.Time
}

func NewLiveHandler(sampleRepository repository.SampleRepository) *LiveHandler {
	return &LiveHandler{
		firstTimestamp: nil,
		lastGameStatus: nil,
		times:          0,
		start:          time.Now(),
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

	// # TODO(joseviccruz): remove this debug.
	elapsed := time.Since(lh.start)
	lh.times++
	fmt.Println("elapsed:", elapsed, "times:", lh.times)
	lh.start = time.Now()

	// enqueue a copy that will be saved into database in another goroutine.
	// lh.samples.Enqueue(proto.Clone(&sample).(*playback.Sample))

	return &sample, nil
}

package world

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"google.golang.org/protobuf/proto"
)

type WorldState struct {
	latestSample          *entity.Sample
	latestSampleMutex     sync.RWMutex
	unsavedSamplesChannel chan entity.Sample
}

var instance *WorldState

var lock sync.Mutex

func GetInstance() *WorldState {
	lock.Lock()
	defer lock.Unlock()
	if instance == nil {
		instance = &WorldState{
			latestSample:      &entity.Sample{},
			latestSampleMutex: sync.RWMutex{},
		}
	}
	return instance
}

func (ws *WorldState) Setup(unsavedSamplesChannel chan entity.Sample) {
	ws.unsavedSamplesChannel = unsavedSamplesChannel
}

func (ws *WorldState) updateLatestSample(identifier []byte, message []byte) error {
	var topic = string(identifier)
	switch topic {
	case service_discovery.GetInstance().GetDetectionWrapperTopic():
		var detectionWrapperProto perception.DetectionWrapper
		if err := proto.Unmarshal(message, &detectionWrapperProto); err != nil {
			return err
		}
		if ws.latestSample.FirstTimestamp.IsZero() {
			ws.latestSample.FirstTimestamp = detectionWrapperProto.Detection.CreatedAt.AsTime()
		}
		ws.latestSample.Timestamp = detectionWrapperProto.Detection.CreatedAt.AsTime()
		ws.latestSample.Detection = *entity.NewDetection(detectionWrapperProto.Detection)
		ws.latestSample.RawDetection = *entity.NewRawDetectionFromRawPackets(detectionWrapperProto.RawDetections)
		ws.latestSample.TrackedDetection = *entity.NewTrackedDetectionFromTrackedPackets(detectionWrapperProto.TrackedDetections)
		return nil
	// case service_discovery.GetInstance().GetRefereeTopic():
	// 	var refereeProto perception.Referee
	// 	if err := proto.Unmarshal(message, &refereeProto); err != nil {
	// 		return err
	// 	}
	// 	ws.latestSample.Referee = entity.NewReferee(refereeProto)
	default:
	}

	return fmt.Errorf("unexpected topic for ZmqMultipartDatagram: '%s'", topic)
}

func (ws *WorldState) UpdateFromDatagram(datagram *network.ZmqMultipartDatagram) {
	ws.latestSampleMutex.Lock()

	defer ws.latestSampleMutex.Unlock()

	err := ws.updateLatestSample(datagram.Identifier, datagram.Message)
	if err != nil {
		fmt.Printf("Error updating latest sample: %v\n", err)
		return
	}
	ws.unsavedSamplesChannel <- (*ws.latestSample)
}

func (ws *WorldState) GetLatestSample() (*entity.Sample, error) {
	ws.latestSampleMutex.RLock()
	defer ws.latestSampleMutex.RUnlock()
	if ws.latestSample == nil {
		return nil, fmt.Errorf("latestSample is nil")
	}
	return ws.latestSample, nil
}

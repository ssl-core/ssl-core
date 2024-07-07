package world

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/proto"
)

type WorldState struct {
	latestSample          *entity.Sample
	latestSampleMutex     sync.RWMutex
	unsavedSamplesChannel chan entity.Sample
	gameEvents            map[time.Time]([]*entity.GameEvent)
	gameEventsMutex       sync.RWMutex
}

var instance *WorldState

var instanceLock sync.Mutex

func GetInstance() *WorldState {
	instanceLock.Lock()
	defer instanceLock.Unlock()
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
		ws.latestSample.UpdateFromPerceptionDetectionWrapper(&detectionWrapperProto)
		return nil
	case service_discovery.GetInstance().GetRefereeTopic():
		var gameStatusProto referee.GameStatus
		if err := proto.Unmarshal(message, &gameStatusProto); err != nil {
			return err
		}
		ws.latestSample.UpdateFromRefereeGameStatus(&gameStatusProto)
		return nil
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

func (ws *WorldState) GetLatestSample() *entity.Sample {
	ws.latestSampleMutex.RLock()
	defer ws.latestSampleMutex.RUnlock()
	return ws.latestSample
}

func (ws *WorldState) GetGameEvents() []*entity.GameEvent {
	ws.gameEventsMutex.RLock()
	defer ws.gameEventsMutex.RUnlock()

	var gameEvents []*entity.GameEvent
	for _, events := range ws.gameEvents {
		gameEvents = append(gameEvents, events...)
	}
	return gameEvents
}

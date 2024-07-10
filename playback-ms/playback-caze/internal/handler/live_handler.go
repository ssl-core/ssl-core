package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
	"google.golang.org/protobuf/proto"
)

type LiveHandler struct {
	samples        chan<- playback.Sample
	lastGameStatus *playback.GameStatus
	//db
}

func NewLiveHandler() *LiveHandler {
	return &LiveHandler{
		samples:        make(chan playback.Sample, 10),
		lastGameStatus: nil,
		// db:    redis_db.NewRedisClient(ChunkStream),
	}
}

func (lh *LiveHandler) Process(datagram *network.ZmqMultipartDatagram) (*playback.Sample, error) {
	topic := string(datagram.Identifier)

	if topic == service_discovery.GetInstance().GetRefereeTopic() {
		var refereeGameStatus referee.GameStatus
		proto.Unmarshal(datagram.Message, &refereeGameStatus)

		lh.lastGameStatus = parseGameStatus(&refereeGameStatus)

		return nil, fmt.Errorf("updating local referee.")
	}

	var sample playback.Sample

	if topic == service_discovery.GetInstance().GetDetectionWrapperTopic() {

	} else {
		return nil, fmt.Errorf("datagram with topic '%s' not processed.", topic)
	}

	// datagram topic -> referee

	return &sample, nil
}

func parseGameStatus(refereeGameStatus *referee.GameStatus) *playback.GameStatus {
	return &playback.GameStatus{
		SourceId:               refereeGameStatus.SourceId,
		Description:            refereeGameStatus.Description,
		Timestamp:              refereeGameStatus.Timestamp,
		MatchType:              refereeGameStatus.MatchType,
		HomeTeam:               parseTeam(refereeGameStatus.HomeTeam),
		AwayTeam:               parseTeam(refereeGameStatus.AwayTeam),
		GameStage:              refereeGameStatus.GameStage,
		GameStageTimeLeft:      refereeGameStatus.GameStageTimeLeft,
		TotalCommandsIssued:    refereeGameStatus.TotalCommandsIssued,
		CommandIssuedTimestamp: refereeGameStatus.CommandIssuedTimestamp,
		Command:                refereeGameStatus.Command,
		NextCommand:            refereeGameStatus.NextCommand,
		GameEvents:             refereeGameStatus.GameEvents,
		GameEventsProposals:    parseGameEventProposals(refereeGameStatus.GameEventsProposals),
	}
}

func parseTeam(team *referee.GameStatus_Team) *playback.GameStatus_Team {
	if team == nil {
		return nil
	}

	return &playback.GameStatus_Team{}
}

func parseGameEventProposals(gameEventsProposal []*referee.GameStatus_GameEventsProposal) []*playback.GameStatus_GameEventsProposal {
	gameEventsProposals := make([]*playback.GameStatus_GameEventsProposal, 0)

	//	for _, proposal := range gameEventsProposals {
	//		gameEventsProposals = append(gameEventsProposals, *playback.GameStatus_GameEventsProposal{
	//			ProposalId:  proposal.ProposalId,
	//			GameEvents:  proposal.GameEvents,
	//			WasAccepted: proposal.WasAccepted,
	//		})
	//	}

	return gameEventsProposals
}

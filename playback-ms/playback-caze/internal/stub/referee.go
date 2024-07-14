package stub

// import (
// 	"fmt"
// 	"sync"
// 	"time"

// 	"github.com/robocin/ssl-core/playback-ms/playback-caze/internal/service_discovery"
// 	"github.com/robocin/ssl-core/common/golang/network"
// 	"github.com/robocin/ssl-core/protocols/common"
// 	"github.com/robocin/ssl-core/protocols/referee"
// 	"google.golang.org/protobuf/proto"
// 	"google.golang.org/protobuf/types/known/durationpb"
// 	"google.golang.org/protobuf/types/known/timestamppb"
// )

// const (
// 	refereeFrequencyHz = 30
// )

// type RefereeStub struct {
// 	SerialId  uint64 `json:"serial_id"`
// 	ticker    *time.Ticker
// 	publisher *network.ZmqPublisherSocket
// }

// func NewRefereeStub() *RefereeStub {
// 	return &RefereeStub{
// 		SerialId:  0,
// 		ticker:    time.NewTicker(time.Second / time.Duration(perceptionFrequencyHz)),
// 		publisher: network.NewZmqPublisherSocket(service_discovery.GetInstance().GetPerceptionAddress()),
// 	}
// }

// func (rs *RefereeStub) makeGameStatusDatagram() network.ZmqMultipartDatagram {
// 	message := rs.makeGameStatusProtobufExample()
// 	messageBytes, err := proto.Marshal(message)
// 	if err != nil {
// 		panic(err)
// 	}
// 	topic := []byte(service_discovery.GetInstance().GetDetectionWrapperTopic())
// 	return network.ZmqMultipartDatagram{
// 		Identifier: topic,
// 		Message:    messageBytes,
// 	}
// }

// func (rs *RefereeStub) makeTeamProtobufExample(color common.RobotId_Color, name string) *referee.GameStatus_Team {
// 	return &referee.GameStatus_Team{
// 		Name:  name,
// 		Score: 3,
// 		RobotIds: []*common.RobotId{
// 			{Number: 1, Color: color},
// 			{Number: 2, Color: color},
// 			{Number: 3, Color: color},
// 		},
// 		GoalkeeperId: &common.RobotId{Number: 4, Color: color},
// 		YellowCards:  2,
// 		TimeToExpireActiveYellowCards: []*durationpb.Duration{
// 			durationpb.New(10 * time.Second),
// 		},
// 		RedCards:                               1,
// 		TimeoutsLeft:                           3,
// 		TotalTimeoutTimeLeft:                   durationpb.New(20 * time.Second),
// 		TotalFoulsCommitted:                    4,
// 		ConsecutiveBallPlacementFailures:       5,
// 		IsBallPlacementEnabled:                 true,
// 		HasBallPlacementFailuresReachedMaximum: false,
// 		MaximumAllowedRobots:                   10,
// 		IsRobotSubstitutionRequested:           false,
// 		IsRobotSubstitutionAllowed:             true,
// 		RobotSubstitutionsLeft:                 4,
// 		RobotSubstitutionTimeLeft:              durationpb.New(30 * time.Second),
// 	}
// }

// func (rs *RefereeStub) makeGameEventsProtobufExample() *common.GameEvent {
// 	return &common.GameEvent{
// 		BallLeftField: &common.GameEvent_BallLeftField{
// 			ByTeam: common.RobotId_COLOR_BLUE,
// 			ByRobot: &common.RobotId{
// 				Number: 1,
// 				Color:  common.RobotId_COLOR_BLUE,
// 			},
// 			LeftFieldPosition: &common.Point2Df{
// 				X: 0,
// 				Y: 0,
// 			},
// 		},
// 		MultipleFouls: &common.GameEvent_MultipleFouls{
// 			ByTeam: common.RobotId_COLOR_YELLOW,
// 		},
// 	}
// }

// func (rs *RefereeStub) makeGameEventsProposalProtobufExample() *referee.GameStatus_GameEventsProposal {
// 	return &referee.GameStatus_GameEventsProposal{
// 		ProposalId:  "proposal_id",
// 		GameEvents:  rs.makeGameEventsProtobufExample(),
// 		WasAccepted: true,
// 	}
// }

// func (rs *RefereeStub) makeGameStatusProtobufExample() *referee.GameStatus {
// 	return &referee.GameStatus{
// 		SourceId:               "source_id",
// 		Timestamp:              timestamppb.Now(),
// 		MatchType:              common.MatchType_MATCH_TYPE_ELIMINATION_PHASE,
// 		HomeTeam:               rs.makeTeamProtobufExample(common.RobotId_COLOR_BLUE, "RobôCIn"),
// 		AwayTeam:               rs.makeTeamProtobufExample(common.RobotId_COLOR_YELLOW, "Tigers"),
// 		GameStage:              common.GameStage_GAME_STAGE_NORMAL_FIRST_HALF,
// 		GameStageTimeLeft:      durationpb.New(600 * time.Second),
// 		TotalCommandsIssued:    10,
// 		CommandIssuedTimestamp: timestamppb.Now(),
// 		Command: &common.GameCommand{
// 			AwayBallPlacement: &common.GameCommand_AwayBallPlacement{
// 				Position: &common.Vector2D{
// 					X: 0,
// 					Y: 0,
// 				},
// 			},
// 			NextCommand: &common.GameCommand{},
// 			GameEvents: []*common.GameEvent{
// 				rs.makeGameEventsProtobufExample(),
// 			},
// 			GameEventsProposals: []*referee.GameStatus_GameEventsProposal{
// 				rs.makeGameEventsProposalProtobufExample(),
// 			},
// 		},
// 	}
// }

// func (rs *RefereeStub) Run(wg *sync.WaitGroup) {
// 	defer rs.ticker.Stop()
// 	defer wg.Done()

// 	fmt.Printf("RefereeStub running...\n")
// 	for range rs.ticker.C {
// 		rs.publisher.Send(rs.makeGameStatusDatagram())
// 	}
// }

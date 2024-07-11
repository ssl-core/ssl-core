package stub

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/types/known/timestamppb"
)

const (
	perceptionFrequencyHz = 120
)

type PerceptionStub struct {
	SerialId  uint64 `json:"serial_id"`
	ticker    *time.Ticker
	publisher *network.ZmqPublisherSocket
}

func NewPerceptionStub() *PerceptionStub {
	return &PerceptionStub{
		SerialId:  0,
		ticker:    time.NewTicker(time.Second / time.Duration(perceptionFrequencyHz)),
		publisher: network.NewZmqPublisherSocket(service_discovery.PerceptionAddress),
	}
}

func (ps *PerceptionStub) makeDetectionWrapperDatagram() network.ZmqMultipartDatagram {
	message := ps.makeDetectionWrapperProtobufExample()
	messageBytes, err := proto.Marshal(message)
	if err != nil {
		panic(err)
	}
	topic := []byte(service_discovery.PerceptionDetectionWrapperTopic)
	return network.ZmqMultipartDatagram{
		Identifier: topic,
		Message:    messageBytes,
	}
}

func (ps *PerceptionStub) makeDetectionWrapperProtobufExample() *perception.DetectionWrapper {
	balls := []*perception.Ball{
		{
			Confidence: 0.9,
			Position: &common.Point3Df{
				X: 123.0,
				Y: 456.0,
				Z: 1.23,
			},
			Velocity: &common.Point3Df{
				X: -987.0,
				Y: 654.0,
				Z: -321.0,
			},
		},
		{
			Confidence: 0.53,
			Position: &common.Point3Df{
				X: -456.0,
				Y: -123.0,
				Z: 987.0,
			},
			Velocity: &common.Point3Df{
				X: 987.0,
				Y: -654.0,
				Z: 321.0,
			},
		},
	}

	robotsBlue := []*perception.Robot{
		{
			Confidence: 0.7,
			RobotId: &common.RobotId{
				Color:  common.RobotId_COLOR_BLUE,
				Number: 0,
			},
			Position: &common.Point2Df{
				X: 123.45,
				Y: -67.89,
			},
			Angle: 3.14159,
			Velocity: &common.Point2Df{
				X: -456.0,
				Y: -123.0,
			},
			AngularVelocity: -1.570795,
			Radius:          90.0,
			Height:          147.5,
			DribblerWidth:   30.0,
			Feedback: &perception.Robot_Feedback{
				Wheels: []*perception.Robot_Feedback_Wheel{
					{
						WheelId:    1,
						MotorSpeed: 123.0,
					},
					{
						WheelId:    2,
						MotorSpeed: 456.0,
					},
					{
						WheelId:    3,
						MotorSpeed: -987.0,
					},
					{
						WheelId:    4,
						MotorSpeed: -321.0,
					},
				},
			},
		},
		{
			Confidence: 0.55,
			RobotId: &common.RobotId{
				Color:  common.RobotId_COLOR_BLUE,
				Number: 1,
			},
			Position: &common.Point2Df{
				X: -45.123,
				Y: 89.67,
			},
			Angle: -3.14159,
			Velocity: &common.Point2Df{
				X: 456.0,
				Y: 123.0,
			},
			AngularVelocity: 1.570795,
			Radius:          87.3,
			Height:          148.5,
			DribblerWidth:   33.0,
			Feedback: &perception.Robot_Feedback{
				Wheels: []*perception.Robot_Feedback_Wheel{
					{
						WheelId:    1,
						MotorSpeed: -123.0,
					},
					{
						WheelId:    2,
						MotorSpeed: -456.0,
					},
					{
						WheelId:    3,
						MotorSpeed: 987.0,
					},
					{
						WheelId:    4,
						MotorSpeed: 321.0,
					},
				},
			},
		},
	}

	robotsYellow := []*perception.Robot{
		{
			Confidence: 0.7,
			RobotId: &common.RobotId{
				Color:  common.RobotId_COLOR_YELLOW,
				Number: 0,
			},
			Position: &common.Point2Df{
				X: 123.45,
				Y: -67.89,
			},
			Angle: 3.14159,
			Velocity: &common.Point2Df{
				X: -456.0,
				Y: -123.0,
			},
			AngularVelocity: -1.570795,
			Radius:          90.0,
			Height:          147.5,
			DribblerWidth:   30.0,
			Feedback: &perception.Robot_Feedback{
				Wheels: []*perception.Robot_Feedback_Wheel{
					{
						WheelId:    1,
						MotorSpeed: 123.0,
					},
					{
						WheelId:    2,
						MotorSpeed: 456.0,
					},
					{
						WheelId:    3,
						MotorSpeed: -987.0,
					},
					{
						WheelId:    4,
						MotorSpeed: -321.0,
					},
				},
			},
		},
	}

	robotsUnspecified := []*perception.Robot{
		{
			Confidence: 0.14,
			RobotId: &common.RobotId{
				Color:  common.RobotId_COLOR_UNSPECIFIED,
				Number: 0,
			},
			Position: &common.Point2Df{
				X: 123.45,
				Y: -67.89,
			},
			Angle: 3.14159,
			Velocity: &common.Point2Df{
				X: -456.0,
				Y: -123.0,
			},
			AngularVelocity: -1.570795,
			Radius:          90.0,
			Height:          147.5,
			DribblerWidth:   30.0,
			Feedback:        &perception.Robot_Feedback{},
		},
	}

	robots := make([]*perception.Robot, 0)
	robots = append(robots, robotsYellow...)
	robots = append(robots, robotsBlue...)
	robots = append(robots, robotsUnspecified...)

	ps.SerialId++
	return &perception.DetectionWrapper{
		Detection: &perception.Detection{
			SerialId:  ps.SerialId,
			CreatedAt: timestamppb.Now(),
			Framerate: 120,
			Balls:     balls,
			Robots:    robots,
		},
	}
}

func (ps *PerceptionStub) Run(wg *sync.WaitGroup) {
	defer ps.ticker.Stop()
	defer wg.Done()

	fmt.Printf("PerceptionStub running...\n")
	for range ps.ticker.C {
		ps.publisher.Send(ps.makeDetectionWrapperDatagram())
	}
}

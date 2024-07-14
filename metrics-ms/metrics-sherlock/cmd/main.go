package main

import (
	"fmt"
	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/metrics-ms/metrics-sherlock/internal/service_discovery"
	"github.com/robocin/ssl-core/protocols/perception"
	"github.com/robocin/ssl-core/protocols/playback"
	"github.com/robocin/ssl-core/protocols/referee"
	"github.com/zeromq/goczmq"
	"google.golang.org/protobuf/proto"
)

const (
	influxDbUrl      = "http://localhost:8086"
	influxDbToken    = "metrics"
	influxDbOrg      = "ssl-core"
	influxDbDatabase = "metrics"

	pollerTimeout = 10 /*ms ~= 100hz*/
)

func receiveFrom(socket *goczmq.Sock) network.ZmqMultipartDatagram {
	if socket == nil {
		return network.ZmqMultipartDatagram{}
	}

	bytes, err := socket.RecvMessageNoWait()

	if len(bytes) == 0 || err != nil {
		return network.ZmqMultipartDatagram{}
	}

	return network.NewZmqMultipartDatagramFromBytes(bytes)
}

func main() {
	// influxdb setup:
	client := influxdb2.NewClient(influxDbUrl, influxDbToken)
	writter := client.WriteAPI(influxDbOrg, influxDbDatabase)

	// zmq services setup:
	perceptionSocket := network.NewZmqSubscriberSocket(
		service_discovery.PerceptionAddress,
		service_discovery.PerceptionDetectionWrapperTopic,
	)

	refereeSocket := network.NewZmqSubscriberSocket(
		service_discovery.RefereeAddress,
		service_discovery.RefereeGameStatusTopic,
	)

	playbackSocket := network.NewZmqSubscriberSocket(
		service_discovery.PlaybackAddress,
		service_discovery.PlaybackSampleTopic,
	)

	poller, err := goczmq.NewPoller()

	if err != nil {
		panic(fmt.Errorf("error creating poller"))
	}

	poller.Add(perceptionSocket.Socket)
	poller.Add(refereeSocket.Socket)
	poller.Add(playbackSocket.Socket)

	// receiving messages:

	// - timer 1s.
	last_time := time.Now()

	packets := map[string]int{}

	i := 0
	for {
		socket := poller.Wait(20) // (pollerTimeout)
		datagram := receiveFrom(socket)

		if datagram.IsEmpty() {
			switch i % 3 {
			case 0:
				packets["perception"] += 1
			case 1:
				packets["referee"] += 1
			case 2:
				packets["playback"] += 1
			}
			i++
			//continue
		}

		topic := string(datagram.Identifier)
		switch topic {
		case service_discovery.PerceptionDetectionWrapperTopic:
			perceptionDetectionWrapper := perception.DetectionWrapper{}
			proto.Unmarshal(datagram.Message, &perceptionDetectionWrapper)

			// here.
			packets["perception"] += 1
		case service_discovery.RefereeGameStatusTopic:
			refereeGameStatus := referee.GameStatus{}
			proto.Unmarshal(datagram.Message, &refereeGameStatus)

			// here.
			packets["referee"] += 1
		case service_discovery.PlaybackSampleTopic:
			playbackSample := playback.Sample{}
			proto.Unmarshal(datagram.Message, &playbackSample)

			// here.
			packets["playback"] += 1
		default:
			// fmt.Println("unexepected topic", topic)
		}

		if time.Since(last_time).Seconds() >= 1.0 {
			fmt.Println(time.Since(last_time))

			point := influxdb2.NewPointWithMeasurement("fps").SetTime(time.Now())

			for service, total := range packets {
				point.AddField(service, map[string]interface{}{"packets": total})
			}
			packets = map[string]int{}

			writter.WritePoint(point)
			writter.Flush()

			last_time = time.Now()
		}
	}
}

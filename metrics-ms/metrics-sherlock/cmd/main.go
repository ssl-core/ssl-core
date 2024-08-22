package main

import (
	"fmt"
	"math"
	"strconv"
	"sync"
	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/robocin/ssl-core/common/golang/concurrency"
	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/common/golang/time_util"
	"github.com/robocin/ssl-core/metrics-ms/metrics-sherlock/internal/service_discovery"
	"github.com/robocin/ssl-core/protocols/perception"
	"github.com/robocin/ssl-core/protocols/playback"
	"github.com/robocin/ssl-core/protocols/referee"
	"github.com/zeromq/goczmq"
	"google.golang.org/protobuf/proto"
)

const (
	influxDbUrl      = "http://metrics-db:8086"
	influxDbToken    = "metrics"
	influxDbOrg      = "ssl-core"
	influxDbDatabase = "metrics"

	pollerTimeout     = 10   /*ms ~= 100hz*/
	dequeueWaitTimeMs = 1000 /*ms ~= 1s*/
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
	fmt.Println("metrics-sherlock is runnning!")

	// influxdb setup:
	client := influxdb2.NewClient(influxDbUrl, influxDbToken)
	writter := client.WriteAPI(influxDbOrg, influxDbDatabase)

	// zmq services setup:
	gatewaySocket := network.NewZmqSubscriberSocket(
		service_discovery.GatewayThirdPartyAddress,
		fmt.Sprintf("%s,%s,%s", service_discovery.GatewayThirdPartyVisionTopic,
			service_discovery.GatewayThirdPartyTrackedTopic,
			service_discovery.GatewayThirdPartyRefereeTopic),
	)

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

	poller.Add(gatewaySocket.Socket)
	poller.Add(perceptionSocket.Socket)
	poller.Add(refereeSocket.Socket)
	poller.Add(playbackSocket.Socket)

	datagrams := concurrency.NewQueue[network.ZmqMultipartDatagram]()
	wg := sync.WaitGroup{}

	// receiving messages:
	wg.Add(1)
	go func() {
		defer wg.Done()

		for {
			socket := poller.Wait(pollerTimeout)
			datagram := receiveFrom(socket)

			if datagram.IsEmpty() {
				continue
			}

			datagrams.Enqueue(datagram)
		}
	}()

	// processing messages:
	wg.Add(1)
	go func() {
		defer wg.Done()

		for {
			raws := 0
			trackeds := 0
			referees := 0
			detections := make([]*perception.Detection, 0)
			gameStatus := make([]*referee.GameStatus, 0)
			samples := make([]*playback.Sample, 0)

			for _, datagram := range datagrams.DequeueAllWait(dequeueWaitTimeMs) {
				topic := string(datagram.Identifier)

				switch topic {
				case service_discovery.GatewayThirdPartyVisionTopic:
					raws++
				case service_discovery.GatewayThirdPartyTrackedTopic:
					trackeds++
				case service_discovery.GatewayThirdPartyRefereeTopic:
					referees++
				case service_discovery.PerceptionDetectionWrapperTopic:
					message := perception.DetectionWrapper{}

					if proto.Unmarshal(datagram.Message, &message) != nil || message.GetDetection() == nil {
						continue
					}

					detections = append(detections, message.GetDetection())
				case service_discovery.RefereeGameStatusTopic:
					message := referee.GameStatus{}

					if proto.Unmarshal(datagram.Message, &message) != nil {
						continue
					}

					gameStatus = append(gameStatus, &message)
				case service_discovery.PlaybackSampleTopic:
					message := playback.Sample{}

					if proto.Unmarshal(datagram.Message, &message) != nil {
						continue
					}

					samples = append(samples, &message)
				default:
					fmt.Println("unexepected topic", topic)
				}
			}

			// writting framerate:
			{
				framerateData := influxdb2.NewPointWithMeasurement("framerate").SetTime(time.Now())
				framerateData.AddField("raw_detection", raws)
				framerateData.AddField("tracked_detection", trackeds)
				framerateData.AddField("game_controller", referees)
				framerateData.AddField("detection", len(detections))
				framerateData.AddField("game_status", len(gameStatus))
				framerateData.AddField("sample", len(samples))
				writter.WritePoint(framerateData)
			}

			// writting balls:
			{
				type Data struct {
					x, y, z, norm float32
					when          time.Time
				}
				balls := map[string][]Data{}
				for _, detection := range detections {
					for idx, ball := range detection.GetBalls() {
						idxStr := strconv.Itoa(idx)
						velocity := ball.GetVelocity()

						data := Data{
							x:    velocity.X,
							y:    velocity.Y,
							z:    velocity.Z,
							norm: float32(math.Sqrt(float64(velocity.X*velocity.X + velocity.Y*velocity.Y + velocity.Z*velocity.Z))),
							when: time_util.TimeFromTimestamp(detection.GetCreatedAt()),
						}

						balls[idxStr] = append(balls[idxStr], data)
					}
				}
				for ballIdx, values := range balls {
					ballData := influxdb2.NewPointWithMeasurement("ball[" + ballIdx + "]")
					for _, data := range values {
						ballData.AddField("v_x", data.x)
						ballData.AddField("v_y", data.y)
						ballData.AddField("v_z", data.z)
						ballData.AddField("v_norm", data.norm)
						ballData.SetTime(data.when)
					}
					writter.WritePoint(ballData)
				}
			}

			writter.Flush()
		}
	}()

	wg.Wait()
}

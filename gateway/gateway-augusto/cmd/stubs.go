package main

import (
	"context"
	"fmt"
	"io"
	"log"
	"sync"
	"time"

	"github.com/golang/protobuf/proto"
	"github.com/golang/protobuf/ptypes/timestamp"
	"github.com/robocin/ssl-core/common/golang/network"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/pkg/pb/gateway"
	"github.com/robocin/ssl-core/gateway/gateway-augusto/pkg/pb/playback"
	"google.golang.org/grpc"
)

func stubBff() {
	fmt.Println("here")
	conn, _ := grpc.Dial("localhost:50051", grpc.WithInsecure())
	defer conn.Close()

	client := gateway.NewGatewayServiceClient(conn)

	ctx, cancel := context.WithTimeout(context.Background(), time.Minute)
	defer cancel()

	stream, _ := client.ReceiveLivestream(ctx)

	for {
		fmt.Println("Sending request")
		request := &gateway.ReceiveLivestreamRequest{}
		if err := stream.Send(request); err != nil {
			log.Fatalf("error sending request", err)
		}

		response, err := stream.Recv()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatalf("Erro receiving response: %v", err)
		}
		log.Printf("Response: %v", response.Sample)
	}
}

func stubThirdParty(proxy chan<- network.ZmqMultipartDatagram, id string, wg *sync.WaitGroup) {
	defer wg.Done()
	for {
		fmt.Println("Sending...", id)
		proxy <- *network.NewZmqMultipartDatagram([]byte("stub-topicsasas"), []byte("stub-message"))
	}
}

func stubMicroserviceAsSubscriber() {
	sub := network.NewZmqSubscriberSocket("ipc:///tmp/.ssl-core/gateway.ipc", "stub-topic")
	for {
		sub.Receive()
	}
}

func stubPlayback() {
	sample := &playback.Sample{
		FirstTimestamp: &timestamp.Timestamp{
			Seconds: time.Now().Unix(),
			Nanos:   0,
		},
	}

	pub := network.NewZmqPublisherSocket("ipc:///tmp/.ssl-core/playback.ipc")
	data, _ := proto.Marshal(sample)
	for {
		pub.Send(*network.NewZmqMultipartDatagram([]byte("topic-playback"), data))
	}
}

func stubSubscriber() {
	sub := network.NewZmqSubscriberSocket("ipc:///tmp/.ssl-core/gateway.ipc", "")
	for {
		datagram := sub.Receive()
		var sample playback.Sample
		err := proto.Unmarshal(datagram.Message, &sample)

		if err != nil {
			fmt.Println("Error")
		}
	}

}

func handler(data network.ZmqMultipartDatagram) network.ZmqMultipartDatagram {
	id := data.Identifier
	if string(data.Message) == "dealer 1" {
		return *network.NewZmqMultipartDatagram(id, []byte("response 1"))
	}

	return *network.NewZmqMultipartDatagram(id, []byte("response 2"))
}

func router() {
	router := network.NewZmqRouterSocket("ipc:///tmp/.ssl-core/dr.ipc")
	for {
		msg := router.Receive()

		fmt.Println(string(msg.Identifier))
		response := handler(msg)
		router.Send(response)
	}
}

func dealer(id string) {
	dealer := network.NewZmqDealerSocket("ipc:///tmp/.ssl-core/dr.ipc")

	for {
		data := *network.NewZmqDatagram([]byte(id))
		dealer.Send(data)

		msg := dealer.Receive()
		fmt.Println("Dealer", string(id), string(msg.Message))
	}
}

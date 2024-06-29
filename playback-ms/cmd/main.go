package main

import (
	"github.com/robocin/ssl-core/playback-ms/internal/application"
	"github.com/robocin/ssl-core/playback-ms/internal/messaging"
	"github.com/robocin/ssl-core/playback-ms/network"
)

func main() {
	datagramChannel := make(chan network.ZmqMultipartDatagram)

	receiver := messaging.NewMessageReceiver(datagramChannel)
	receiver.Run()

	sender := messaging.MessageSender{}
	sender.Setup()

	sampleController := application.NewSampleController(sender, datagramChannel)
	sampleController.Run()
}

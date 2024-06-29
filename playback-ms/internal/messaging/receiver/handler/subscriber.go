package handler

import "github.com/robocin/ssl-core/playback-ms/network"

type SubscriberHandler struct {
	socket  *network.ZmqSubscriberSocket
	channel *chan network.ZmqMultipartDatagram
}

func NewSubscriberHandler(socket *network.ZmqSubscriberSocket, channel *chan network.ZmqMultipartDatagram) *SubscriberHandler {
	return &SubscriberHandler{socket, channel}
}

func (s *SubscriberHandler) receive() network.ZmqMultipartDatagram {
	return s.socket.Receive()
}

func (s *SubscriberHandler) close() {
	s.socket.Close()
}

func (s *SubscriberHandler) Handle() {
	defer s.close()
	for {
		datagram := s.receive()
		(*s.channel) <- datagram
	}
}

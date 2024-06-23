package network

type ZmqDatagram struct {
	topic   string
	message []byte
}

func NewZmqDatagram(topic string, message []byte) *ZmqDatagram {
	return &ZmqDatagram{
		topic:   topic,
		message: message,
	}
}

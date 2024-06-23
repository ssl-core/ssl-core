package network

type ZmqDatagram struct {
	Topic   string
	Message []byte
}

func NewZmqDatagram(topic string, message []byte) *ZmqDatagram {
	return &ZmqDatagram{
		Topic:   topic,
		Message: message,
	}
}

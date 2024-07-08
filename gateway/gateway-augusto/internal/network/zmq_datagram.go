package network

type ZmqDatagram struct {
	Message []byte
}

func NewZmqDatagram(message []byte) *ZmqDatagram {
	return &ZmqDatagram{
		Message: message,
	}
}

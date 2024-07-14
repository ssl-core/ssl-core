package network

type ZmqMultipartDatagram struct {
	Identifier []byte
	Message    []byte
}

func NewZmqMultipartDatagram(identity []byte, message []byte) ZmqMultipartDatagram {
	return ZmqMultipartDatagram{
		Identifier: identity,
		Message:    message,
	}
}

func (zmd *ZmqMultipartDatagram) IsEmpty() bool {
	return len(zmd.Identifier) == 0 || len(zmd.Message) == 0
}

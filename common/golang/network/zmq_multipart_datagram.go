package network

type ZmqMultipartDatagram struct {
	Identifier []byte
	Message    []byte
}

func NewZmqMultipartDatagramFromBytes(bytes [][]byte) ZmqMultipartDatagram {
	if len(bytes) != 2 {
		return ZmqMultipartDatagram{}
	}
	return ZmqMultipartDatagram{
		Identifier: bytes[0],
		Message:    bytes[1],
	}
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

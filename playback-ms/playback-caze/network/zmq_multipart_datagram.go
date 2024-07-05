package network

type ZmqMultipartDatagram struct {
	Identifier []byte
	Message    []byte
}

func NewZmqMultipartDatagram(identity []byte, message []byte) *ZmqMultipartDatagram {
	return &ZmqMultipartDatagram{
		Identifier: identity,
		Message:    message,
	}
}

func (zmd *ZmqMultipartDatagram) IsEmpty() bool {
	return zmd.Identifier == nil || zmd.Message == nil
}

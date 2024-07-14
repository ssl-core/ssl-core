package application

type ConnectionProxyEvent struct {
	Type    string
	Payload interface{}
}

func NewConnectionProxyEvent(eventType string, payload interface{}) ConnectionProxyEvent {
	return ConnectionProxyEvent{
		Type:    eventType,
		Payload: payload,
	}
}

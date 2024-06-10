package application

type ConnectionProxyListener interface {
	OnNotify(event ConnectionProxyEvent)
}

type ConnectionProxy struct {
	listeners []ConnectionProxyListener
}

func NewConnectionProxy() *ConnectionProxy {
	return &ConnectionProxy{}
}

func (pp *ConnectionProxy) AddListener(listener ConnectionProxyListener) {
	pp.listeners = append(pp.listeners, listener)
}

func (pp *ConnectionProxy) Notify(event ConnectionProxyEvent) {
	for _, listener := range pp.listeners {
		listener.OnNotify(event)
	}
}

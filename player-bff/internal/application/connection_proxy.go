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

func (pp *ConnectionProxy) RemoveListener(listener ConnectionProxyListener) {
	for i, l := range pp.listeners {
		if l == listener {
			pp.listeners = append(pp.listeners[:i], pp.listeners[i+1:]...)
			break
		}
	}
}

func (pp *ConnectionProxy) Notify(event ConnectionProxyEvent) {
	for _, listener := range pp.listeners {
		listener.OnNotify(event)
	}
}

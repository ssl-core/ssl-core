package worker

import (
	"net"

	"github.com/robocin/ssl-core/common/golang/network"
)

const protocol = "udp"

type UdpMulticastWorker struct {
	conn      *net.UDPConn
	size      int
	proxy     chan<- network.ZmqMultipartDatagram
	id        []byte
	onReceive func(datagram network.ZmqMultipartDatagram)
}

func NewUdpMulticastWorker(address string, size int, proxy chan<- network.ZmqMultipartDatagram, id string, onReceive func(datagram network.ZmqMultipartDatagram)) *UdpMulticastWorker {

	addr, err := net.ResolveUDPAddr(protocol, address)
	if err != nil {
		panic(err)
	}

	conn, err := net.ListenMulticastUDP(protocol, nil, addr)
	if err != nil {
		panic(err)
	}

	if onReceive == nil {
		onReceive = func(datagram network.ZmqMultipartDatagram) {
			// Default behavior: no-op
		}
	}

	return &UdpMulticastWorker{
		conn:      conn,
		size:      size,
		proxy:     proxy,
		id:        []byte(id),
		onReceive: onReceive,
	}
}

func (w *UdpMulticastWorker) Listen() {
	buffer := make([]byte, w.size)
	for {
		bytes, _, err := w.conn.ReadFromUDP(buffer)

		if err != nil {
			continue
		}

		datagram := network.NewZmqMultipartDatagram(w.id, buffer[:bytes])
		w.proxy <- datagram
		w.onReceive(datagram)
	}
}

func (w *UdpMulticastWorker) Close() {
	w.conn.Close()
}

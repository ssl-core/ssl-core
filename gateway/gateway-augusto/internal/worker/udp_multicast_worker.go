package worker

import (
	"net"

	"github.com/robocin/ssl-core/common/golang/network"
)

const protocol = "udp"

type UdpMulticastWorker struct {
	conn  *net.UDPConn
	size  int
	proxy chan<- network.ZmqMultipartDatagram
	id    []byte
}

func NewUdpMulticastWorker(address string, size int, proxy chan<- network.ZmqMultipartDatagram, id string) *UdpMulticastWorker {
	addr, err := net.ResolveUDPAddr(protocol, address)
	if err != nil {
		panic(err)
	}

	conn, err := net.ListenMulticastUDP(protocol, nil, addr)
	if err != nil {
		panic(err)
	}

	return &UdpMulticastWorker{
		conn:  conn,
		size:  size,
		proxy: proxy,
		id:    []byte(id),
	}
}

func (w *UdpMulticastWorker) Listen() {
	buffer := make([]byte, w.size)
	for {
		bytes, _, err := w.conn.ReadFromUDP(buffer)

		if err != nil {
			continue
		}

		w.proxy <- network.NewZmqMultipartDatagram(w.id, buffer[:bytes])
	}
}

func (w *UdpMulticastWorker) Close() {
	w.conn.Close()
}

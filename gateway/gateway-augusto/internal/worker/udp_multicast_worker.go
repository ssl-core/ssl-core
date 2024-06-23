package worker

import (
	"net"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
)

const protocol = "udp"

type UdpMulticastWorker struct {
	conn  *net.UDPConn
	size  int
	proxy chan<- network.ZmqDatagram
	id    string
}

func NewUdpMulticastWorker(address string, size int, proxy chan<- network.ZmqDatagram, id string) *UdpMulticastWorker {
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
		id:    id,
	}
}

func (c *UdpMulticastWorker) Listen() {
	buffer := make([]byte, c.size)
	for {
		bytes, _, err := c.conn.ReadFromUDP(buffer)

		if err != nil {
			continue
		}

		c.proxy <- *network.NewZmqDatagram(c.id, buffer[:bytes])
	}
}

func (c *UdpMulticastWorker) Close() {
	c.conn.Close()
}

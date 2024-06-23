package worker

import (
	"net"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/network"
)

const protocol = "udp"

type UdpMulticastWorker struct {
	Conn  *net.UDPConn
	Size  int
	Proxy chan<- network.ZmqDatagram
	Id    string
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
		Conn:  conn,
		Size:  size,
		Proxy: proxy,
		Id:    id,
	}
}

func (c *UdpMulticastWorker) Listen() {
	buffer := make([]byte, c.Size)
	for {
		bytes, _, err := c.Conn.ReadFromUDP(buffer)

		if err != nil {
			continue
		}

		c.Proxy <- *network.NewZmqDatagram(c.Id, buffer[:bytes])
	}
}

func (c *UdpMulticastWorker) Close() {
	c.Conn.Close()
}

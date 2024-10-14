#ifndef ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_SENDER_H
#define ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_SENDER_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_arpa_inet) and __robocin_lib_arpa_inet >= 202405L

#include <string>

namespace robocin {

class IUdpMulticastSocketSender {
  public:
    using send_type = std::string;

    IUdpMulticastSockerSender() = default;

    IUdpMulticastSockerSender(const IUdpMulticastSocketSender&) = delete;
    IUdpMulticastSockerSender& operator=(const IUdpMulticastSocketSender&) = delete;
    IUdpMulticastSockerSender(IUdpMulticastSocketSender&&) = default;
    IUdpMulticastSockerSender& operator=(IUdpMulticastSocketSender&&) = default;  

    virtual ~IUdpMulticastSockerSender() = default;

    virtual void connect(std::string_view ip_address, int port) const = 0;
    virtual void send(const send_type& message) const = 0;
    virtual void close() const = 0;

    [[nodiscard]] virtual int fd() const = 0;
};

class UdpMulticastSocketSender : public IUdpMulticastSocketSender {
  public:
    explicit UdpMulticastSocketSender(size_t size = 1024);

    void connect(std::string_view ip_address, int port) const override;
    void send(const send_type& message) const override;
    void close() const override;

    [[nodiscard]] int fd() const override;

  private:
    int fd_;
    size_t size_;
};

}

#endif

#endif // ROBOCIN_NETWORK_UDP_MULTICAST_SOCKET_SENDER_H
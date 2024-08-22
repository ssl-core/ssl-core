# Experiment: Proxy Multiclient Single Service Gateway

This experiments aims to validate the behavior of multiple clients that simulate a Backend For Frontend (BFF) module connecting with a gateway that uses a [ZeroMQ Proxy Pattern](https://zguide.zeromq.org/docs/chapter2/#ZeroMQ-s-Built-In-Proxy-Function) and receiving replies from a single service.

## Table of Contents
- [Client](#Client)
- [Gateway](#Gateway)
- [Service](#Service)
- [Testing](#Testing)

## [Client](proxy-multiclient-single-service-gateway/client_main.cpp)

The client is a standard client implementation that uses a inter-process communication and a ZMQ_REQ socket, one type of socket made for client-server communication, allowing the client to send requests and receive replies.

## [Gateway](proxy-multiclient-single-service-gateway/gateway_main.cpp)

The gateway implements a frontend socket (ZMQ_ROUTER) and a backend socket (ZMQ_DEALER) to properly route the requests made from some client to the service and the response given by the service to the client. 

It is based on the [extended reply envelope](https://zguide.zeromq.org/docs/chapter3/#The-Extended-Reply-Envelope), where each client connected with the frontend socket receive a random identity that allows the gateway to recognize which client is waiting for a specific response.

The [proxy implementation](https://libzmq.readthedocs.io/en/zeromq3-x/zmq_proxy.html) provided by the ZeroMQ library creates a similar behavior of adding a poller responsible to track events from both ZMQ_ROUTER and ZMQ_DEALER socket. When receiving a request from a client in the frontend socket, it sends to the service using the backend socket. Similarly, when receiving a response in the backend socket, it sends to the appropriate client using the frontend socket (see code below).

```C++
std::array<zmq::pollitem_t, 2> items = {
    zmq::pollitem_t{router, 0, ZMQ_POLLIN, 0},
    zmq::pollitem_t{dealer, 0, ZMQ_POLLIN, 0},
};

while (true) {
  zmq::poll(items);

  if (items[0].revents & ZMQ_POLLIN) {
    zmq::multipart_t messages;
    auto _ = zmq::recv_multipart(router, std::back_inserter(messages), zmq::recv_flags::none);

    zmq::send_multipart(dealer, messages, zmq::send_flags::none);
  }

  if (items[1].revents & ZMQ_POLLIN) {
    zmq::multipart_t messages;
    auto _ = zmq::recv_multipart(dealer, std::back_inserter(messages), zmq::recv_flags::none);

    zmq::send_multipart(router, messages, zmq::send_flags::none);
  }
}
```

## [Service](proxy-multiclient-single-service-gateway/service_main.cpp)

The service also uses inter-process communication and a ZMQ_DEALER socket to ensure asynchronicity and receive the package sent by the gateway as a multipart message that represents the fields of the extended reply envelope: client's identity, delimiter and message (request).

As response, it only sends back the same received message.

## Testing

To test the behavior, build the container and run the executables located in `bin` folder in the following order:

``` bash
./gateway_main
./service_main
./client_main <id>
```

To add multiple clients, just run `client_main` as many times as needed giving each an unique identifier to differentiate them.

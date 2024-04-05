# tester

TODO([#41](https://github.com/robocin/ssl-core/issues/41)): Add a description of the tester directory.

```json
{
    "requests": [
        {
            "count": "30", // optional
            "fps": "60", // optional
            // zmq or udp_multicast
            "zmq": {
                "address": "ipc:///tmp/vision-service",
                "topic": "vision-tester"
            },
            "udp_multicast": {
                "ip": "224.5.23.2",
                "port": "10020"
            },
            "message_path_pb": "message-vision.textpb", // required
            "protobuf_type": "SSL_WrapperPacket", // required
            "response": { // optional
                // zmq or udp_multicast
                "zmq": {
                    "address": "ipc:///tmp/vision-service",
                    "topic": "vision-tester"
                },
                "udp_multicast": {
                    "ip": "224.5.23.2",
                    "port": "10020",
                },
                "message_path_pb": "message-vision.textpb", // required
                "protobuf_type": "SSL_WrapperPacket" // required
            }
        }
    ]
}

```

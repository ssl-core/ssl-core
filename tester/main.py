import json
from multicast_udp_socket import MulticastUdpSocket
from tester import Tester
from testerCommonFunctions import pbMessage
import time

# reading json data with service infos
fileName = "tester-input-data.json"
f = open(fileName)

# criar enum com os atributos do json
data = json.load(f)
servicesList = []
pbFolder = "messages-pb"
for i in data["requests"]:
    response = ""
    count = i["count"]
    pbType = i["protobuf_type"]
    pbPath = "{}/{}".format(pbFolder, i["message_path_pb"])
    zqm_address = i["zmq"]["address"]
    zmq_topic = i["zmq"]["topic"]
    udp_multicast_ip = i["udp_multicast"]["ip"]
    udp_multicast_port = int(i["udp_multicast"]["port"])
    # udp_multicast_inet = str(i["udp_multicast"]["inet"])

    if i["response"]:
        response = i["response"]

    multicast_udp = MulticastUdpSocket()
    # multicast_udp.bind(udp_multicast_ip, udp_multicast_port, udp_multicast_inet)
    messagePb = pbMessage(pbPath, pbType)

    while True:
        multicast_udp.send(messagePb, udp_multicast_ip, udp_multicast_port)
        time.sleep(0.5)

    exit(0)

    tester = Tester(
        count,
        pbType,
        pbPath,
        zqm_address,
        zmq_topic,
        response,
        udp_multicast_ip,
        udp_multicast_port,
        udp_multicast_inet,
    )
    tester.createZmqSocket()
    tester.sendMessage()
    tester.getLatency()
    servicesList.append(tester)

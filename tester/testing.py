import json
from tester import Tester

# reading json data with service infos
fileName = "tester-data.json"
f = open(fileName)

data = json.load(f)
servicesList = []
pbFolder = "messages-pb/"
for i in data["requests"]:
    response = ""
    count = i["count"]
    pbType = i["protobuf_type"]
    pbPath = "{}{}".format(pbFolder, i["message_path_pb"])
    address = i["address_zmq"]
    topic = i["topic"]
    if i["response"]:
        response = i["response"]
    tester = Tester(count, pbType, pbPath, address, topic, response)
    tester.createSocket()
    tester.sendMessage()
    tester.latencyMean()
    servicesList.append(tester)

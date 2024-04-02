from datetime import datetime


def measuringTime(init, end):
    t1 = datetime.fromtimestamp(init)
    t2 = datetime.fromtimestamp(end)
    delta = t2 - t1
    diffTime = delta.total_seconds() * 1000
    return [
        "The latency time is: {} ms".format(diffTime),
        diffTime,
    ]


def latencyMean(latencyList):
    listDiffTime = latencyList
    latencySum = sum(listDiffTime)
    mean = latencySum / len(listDiffTime)
    return mean

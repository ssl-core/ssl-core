from datetime import datetime


def measure_time(init, end):
    t1 = datetime.fromtimestamp(init)
    t2 = datetime.fromtimestamp(end)
    delta = t2 - t1
    diffTime = delta.total_seconds() * 1000
    return [
        "The latency time is: {} ms".format(diffTime),
        diffTime,
    ]


def measure_mean(latencies):
    return sum(latencies) / len(latencies)

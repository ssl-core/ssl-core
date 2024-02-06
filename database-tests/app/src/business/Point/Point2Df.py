import json

class Point2Df:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return json.dumps(self.to_dict())

    def to_dict(self):
        return {"x": self.x, "y": self.y}

    @staticmethod
    def from_string(point_string):
        # Assuming the format of point_string is "x,y"
        x, y = map(float, point_string.split(','))
        return Point2Df(x, y)

import json

class Point3Df:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return json.dumps(self.to_dict())

    def to_dict(self):
        return {"x": self.x, "y": self.y, "z": self.z}

    @staticmethod
    def from_string(point_string):
        # Assuming the format of point_string is "x,y,z"
        x, y, z = map(float, point_string.split(','))
        return Point3Df(x, y, z)

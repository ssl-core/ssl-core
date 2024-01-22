from src.business.Point.Point2Df import Point2Df
from src.business.Point.Point3Df import Point3Df
from src.business.Source.Source import Source

class Ball:
    def __init__(self, uuid, source: Source, position: Point2Df, velocity: Point3Df):
        self.uuid = uuid
        self.source = source
        self.position = position
        self.velocity = velocity

    def __str__(self):
        return f'Ball(uuid={self.uuid}, source={self.source.name}, position={self.position}, velocity={self.velocity})'

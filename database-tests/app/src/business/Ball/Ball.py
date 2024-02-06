from src.business.Point.Point2Df import Point2Df
from src.business.Point.Point3Df import Point3Df
from src.business.Source.Source import Source

import json

class Ball:
    def __init__(self, uuid, source: Source, position: Point2Df, velocity: Point3Df):
        self.uuid = uuid
        self.source = source
        self.position = position
        self.velocity = velocity

    def __str__(self):
        return json.dumps(self.to_dict())
    
    @staticmethod
    def from_string(ball_string):
        # Parse the string into a dictionary
        ball_dict = json.loads(ball_string)

        # Extract values from the dictionary
        uuid = ball_dict['uuid']
        source = ball_dict['source']['name']
        position = ball_dict['position']
        velocity = ball_dict['velocity']

        # Create and return a Ball object
        return Ball(uuid, source, position, velocity)
    
    @staticmethod
    def from_dict(ball_dict):
        uuid = ball_dict['uuid']
        source = ball_dict['source']
        position = ball_dict['position']
        velocity = ball_dict['velocity']
        return Ball(uuid, source, position, velocity)
    
    def to_dict(self):
        return {
            "uuid": self.uuid,
            "source": self.source.to_dict(),
            "position": self.position.to_dict(),
            "velocity": self.velocity.to_dict()
        }

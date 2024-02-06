from src.business.Point.Point2Df import Point2Df
from src.business.Source.Source import Source
from .Color import Color

import json

class Robot:
    def __init__(self,
                 uuid: str,
                 id: int,
                 color: Color,
                 position: Point2Df,
                 angle: float,
                 velocity: Point2Df,
                 angular_velocity: float,
                 source: Source):
        self.uuid = uuid
        self.id = id
        self.color = color
        self.position = position
        self.angle = angle
        self.velocity = velocity
        self.angular_velocity = angular_velocity
        self.source = source

    def __str__(self) -> str:
        return json.dumps(self.to_dict())
    
    @staticmethod
    def from_dict(robot_dict):
        uuid = robot_dict['uuid']
        id = robot_dict['id']
        color = robot_dict['color']
        position = robot_dict['position']
        angle = robot_dict['angle']
        velocity = robot_dict['velocity']
        angular_velocity = robot_dict['angular_velocity']
        source = robot_dict['source']
        return Robot(uuid, id, color, position, angle, velocity, angular_velocity, source)

    @staticmethod
    def from_string(robot_string):
        robot_dict = json.loads(robot_string)

        uuid = robot_dict['uuid']
        id = robot_dict['id']
        color = robot_dict['color']
        position = robot_dict['position']
        angle = robot_dict['angle']
        velocity = robot_dict['velocity']
        angular_velocity = robot_dict['angular_velocity']
        source = robot_dict['source']

        return Robot(uuid, id, color, position, angle, velocity, angular_velocity, source)
    
    def to_dict(self):
        return {
            "uuid": self.uuid,
            "id": self.id,
            "color": self.color.name,
            "position": self.position.to_dict(),
            "angle": self.angle,
            "velocity": self.velocity.to_dict(),
            "angular_velocity": self.angular_velocity,
            "source": self.source.to_dict()
        }
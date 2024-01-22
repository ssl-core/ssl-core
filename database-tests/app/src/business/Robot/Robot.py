from src.business.Point.Point2Df import Point2Df
from src.business.Source.Source import Source
from .Color import Color

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
        return f'Robot(uuid={self.uuid}, id={self.id}, color={self.color.name}, position={self.position}, angle={self.angle}, velocity={self.velocity}, angular_velocity={self.angular_velocity}, source={self.source.name})'
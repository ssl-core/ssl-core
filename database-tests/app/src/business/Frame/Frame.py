from src.business.Ball.Ball import Ball
from src.business.Robot.Robot import Robot
from src.business.Field.Field import Field

import json

class Frame:
    def __init__(self, id: str, balls: [Ball], robots: [Robot], field: Field):
        self.id = id
        self.balls = balls
        self.robots = robots
        self.field = field

    def to_dict(self):
        balls_dict = [ball.to_dict() for ball in self.balls]
        robots_dict = [robot.to_dict() for robot in self.robots]
        field_dict = self.field.to_dict()

        return {
            "id": self.id,
            "balls": balls_dict,
            "robots": robots_dict,
            "field": field_dict
        }

    @staticmethod
    def from_string(frame_string):
        frame_dict = json.loads(frame_string)

        id = frame_dict['id']
        balls = [Ball.from_dict(ball_dict) for ball_dict in frame_dict['balls']]
        robots = [Robot.from_dict(robot_dict) for robot_dict in frame_dict['robots']]
        field = Field.from_dict(frame_dict['field'])

        return Frame(id, balls, robots, field)

    def __str__(self):
        return json.dumps(self.to_dict())
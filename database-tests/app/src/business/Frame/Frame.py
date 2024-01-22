from src.business.Ball.Ball import Ball
from src.business.Robot.Robot import Robot
from src.business.Field.Field import Field

class Frame:
    def __init__(self, id: str, balls: [Ball], robots: [Robot], field: Field):
        self.id = id
        self.ball = balls
        self.robots = robots
        self.field = field

    def __str__(self):
        return f'Frame(id={self.id}, balls={self.ball}, robots={self.robots}, field={self.field})'
import enum


class PubSubMode(enum.Enum):
    Wait = enum.auto()
    DontWait = enum.auto()

from enum import Enum

class Source(Enum):
    SIMULATION = 1
    SSL_VISION = 2

    def to_dict(self):
        return self.name
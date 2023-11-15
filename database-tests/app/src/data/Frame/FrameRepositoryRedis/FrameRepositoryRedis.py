import redis
import json
from src.data.Frame.IFrameRepository import IFrameRepository
from src.business.Frame.Frame import Frame

class FrameRepositoryRedis(IFrameRepository):
    def __init__(self):
        super().__init__()
        try:
            self.redis = redis.Redis(host='redis', port=6379, db=0)
        except redis.ConnectionError:
            print("Could not connect to Redis server.")

    def save(self, frame: Frame) -> None:
        frame_id = frame.id
        try:
            frame_data = str(frame)
        except TypeError:
            print("Frame object contains non-serializable attributes.")
        else:
            self.redis.set(frame_id, frame_data)

    def find(self, frame_id: str) -> Frame:
        frame_data = self.redis.get(frame_id)
        if frame_data is not None:
            return Frame(**json.loads(frame_data))
        else:
            print("Frame not found.")
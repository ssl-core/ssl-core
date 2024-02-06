import redis
import json
from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.business.Frame.Frame import Frame

class FrameRepositoryRedisKeyValue(IFrameRepositoryKeyValue):
    def __init__(self):
        super().__init__()
        try:
            self._name = "redis - Key Value"

            self.redis = redis.Redis(host='redis', port=6379, db=0)

            print("Flush the database. (redis)")
            self.redis.flushdb()
        except redis.ConnectionError:
            print("Could not connect to Redis server.")

    def repository_name(self) -> str:
        return self._name

    def delete_all(self) -> None:
        print("Flush the database. (redis)")
        self.redis.flushdb()

    def get_data_size(self):
        # get the data size used by the database
        return self.redis.dbsize()

    def save(self, frame_id: str, frame:  str) -> None:
        # frame_id = frame.id
        # try:
        #     frame = str(frame)
        # except TypeError:
        #     print("Frame object contains non-serializable attributes.")
        # else:
        self.redis.set(frame_id, frame)

    def find(self, frame_id: str) -> Frame:
        frame_data = self.redis.get(frame_id)
        if frame_data is not None:
            return frame_data.value
        else:
            print("Frame not found.")

    def update(self, frame: Frame) -> Frame:
        pass

    def delete(self, frame: Frame) -> None:
        pass

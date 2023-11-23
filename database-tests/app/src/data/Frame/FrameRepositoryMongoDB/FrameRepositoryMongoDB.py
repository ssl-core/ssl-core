import pymongo
from pymongo import MongoClient
import json
from src.data.Frame.IFrameRepository import IFrameRepository
from src.business.Frame.Frame import Frame

class FrameRepositoryMongoDB(IFrameRepository):
    def __init__(self):
        super().__init__()
        try:
            self.client = MongoClient('mongodb://mongodb:27017/') 
            self.db = self.client['frames_database']
            self.collection = self.db['frames_collection']
        except pymongo.errors.ConnectionFailure:
            print("Could not connect to MongoDB server.")

    def save(self, frame: Frame) -> None:
        frame_id = frame.id
        try:
            frame_data = str(frame)
        except TypeError:
            print("Frame object contains non-serializable attributes.")
        else:
            frame_dict = {'_id': frame_id, 'data': frame_data}
            self.collection.insert_one(frame_dict)

    def find(self, frame_id: str) -> Frame:
        frame_dict = self.collection.find_one({'_id': frame_id})
        if frame_dict is not None:
            return Frame(**json.loads(frame_dict['data']))
        else:
            print("Frame not found.")

import pymongo
from pymongo import MongoClient
import json
from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.business.Frame.Frame import Frame

class FrameRepositoryMongoDBKeyValue(IFrameRepositoryKeyValue):
    def __init__(self):
        super().__init__()
        try:
            self._name = "MongoDB - Key Value"

            self.client = MongoClient('mongodb://mongodb:27017/') 
            self.db = self.client['frames_database']
            self.collection = self.db['frames_collection']

        except pymongo.errors.ConnectionFailure:
            print("Could not connect to MongoDB server.")

    def repository_name(self) -> str:
        return self._name
    
    def delete_all(self) -> None:
        print("Drop the collection if it exists. (mongoDB - Key Value)")
        self.collection.delete_many({})

    def get_data_size(self):
        return self.db.command("dbstats")['dataSize']
    
    # methods for document database
    def save(self, frame_id: str, frame: str) -> None:
        self.collection.insert_one({'_id': frame_id, 'data': frame})

    def find(self, frame_id: str) -> Frame:
        frame_cursor = self.collection.find({'_id': frame_id})
        print("frame_cursor: {frame_dict}".format(frame_dict=frame_cursor[0]))
        return Frame.from_string(frame_cursor[0]['data'])
        # if frame_dict is not None:
        #     return Frame(**json.loads(frame_dict['data']))
        # else:
        #     print("Frame not found.")

    def update(self, frame: Frame) -> Frame:
        pass

    def delete(self, frame: Frame) -> None: 
        pass


    
    
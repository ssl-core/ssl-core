import pymongo
from pymongo import MongoClient
import json
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from src.business.Frame.Frame import Frame

class FrameRepositoryMongoDBDocument(IFrameRepositoryDocument):
    # common to MongoDB variants
    # TODO ? create base class for MongoDB variants
    def __init__(self):
        super().__init__()
        try:
            self._name = "mongoDB - Document"

            self.client = MongoClient('mongodb://mongodb:27017/') 
            self.db = self.client['frames_database']
            self.collection = self.db['frames_collection']

            print("Drop the collection if it exists. (mongoDB)")
            self.collection.delete_many({})

        except pymongo.errors.ConnectionFailure:
            print("Could not connect to MongoDB server.")

    def repository_name(self) -> str:
        return self._name

    def delete_all(self) -> None:
        print("Drop the collection if it exists. (mongoDB)")
        self.collection.delete_many({})

    def get_data_size(self):
        return self.db.command("dbstats")['dataSize']

    # methods for document database 
    def save(self, frame: Frame):
        frame_document = {
            '_id': frame.id,
            'balls': [
                {
                    'uuid': ball.uuid,
                    'source': ball.source.value,
                    'position': {
                        'x': ball.position.x,
                        'y': ball.position.y
                    },
                    'velocity': {
                        'x': ball.velocity.x,
                        'y': ball.velocity.y,
                        'z': ball.velocity.z
                    }
                } for ball in frame.ball
            ],
            'robots': [
                {
                    'uuid': robot.uuid,
                    'id': robot.id,
                    'color': robot.color.value,
                    'position': {
                        'x': robot.position.x,
                        'y': robot.position.y
                    },
                    'angle': robot.angle,
                    'velocity': {
                        'x': robot.velocity.x,
                        'y': robot.velocity.y
                    },
                    'angular_velocity': robot.angular_velocity,
                    'source': robot.source.value
                } for robot in frame.robots
            ],
            'field': {
                'length': frame.field.length,
                'width': frame.field.width,
                'goal_depth': frame.field.goal_depth,
                'goal_width': frame.field.goal_width,
                'penalty_area_depth': frame.field.penalty_area_depth,
                'penalty_area_width': frame.field.penalty_area_width,
                'boundary_width': frame.field.boundary_width,
                'goal_center_to_penalty_mark': frame.field.goal_center_to_penalty_mark
            }
        }

        self.collection.insert_one(frame_document)

    def find(self, frame_id: str) -> Frame:
        frame_dict = self.collection.find_one({'_id': frame_id})
        if frame_dict is not None:
            return Frame(**json.loads(frame_dict['data']))
        else:
            print("Frame not found.")

    def update(self, frame: Frame) -> bool:
        pass

    def delete(self, frame: Frame) -> bool: 
        pass


    
    
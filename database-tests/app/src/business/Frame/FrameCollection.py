from src.business.Frame.Frame import Frame
from src.data.Frame.IFrameRepository import IFrameRepository

class FrameCollection:
    def __init__(self, frame_repository: IFrameRepository):
        self.frame_repository = frame_repository

    def create(self, frame: Frame):
        self.frame_repository.create(frame)

    def find_frame(self, frame_id: str) -> Frame:
        return self.frame_repository.find(frame_id)

    def update_frame(self, frame: Frame):
        self.frame_repository.update(frame)
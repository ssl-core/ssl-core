from src.business.Frame.Frame import Frame
import FrameCollection

class FrameController:
    def __init__(self, frame_collection: FrameCollection):
        self.frame_collection = frame_collection

    def find_frame(self, id: str) -> Frame:
        return self.frame_collection.find_frame(id)

    def update_frame(self, frame: Frame):
        return self.frame_collection.update_frame(frame)
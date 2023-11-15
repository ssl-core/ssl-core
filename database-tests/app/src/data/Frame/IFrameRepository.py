from src.business.Frame.Frame import Frame

class IFrameRepository:
    def __init__(self) -> None:
        pass

    def save(self, frame: Frame) -> None:
        raise NotImplementedError

    def find(self, frame_id: str) -> Frame:
        raise NotImplementedError

    def update(self, frame: Frame) -> Frame:
        raise NotImplementedError

    def delete(self, frame: Frame) -> None:
        raise NotImplementedError
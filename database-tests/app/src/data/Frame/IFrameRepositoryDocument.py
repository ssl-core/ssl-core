from src.business.Frame.Frame import Frame

class IFrameRepositoryDocument:
    def __init__(self) -> None:
        pass

    def repository_name(self) -> str:
        raise NotImplementedError

    def save(self, frame: Frame) -> None:
        raise NotImplementedError

    def find(self, frame_id: str) -> Frame:
        raise NotImplementedError

    def update(self, frame: Frame) -> Frame:
        raise NotImplementedError

    def delete(self, frame: Frame) -> None:
        raise NotImplementedError
    
    def delete_all(self) -> None:
        raise NotImplementedError

    def get_data_size(self) -> int:
        raise NotImplementedError
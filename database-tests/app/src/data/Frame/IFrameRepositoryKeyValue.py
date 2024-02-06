from src.business.Frame.Frame import Frame

class IFrameRepositoryKeyValue:
    def __init__(self) -> None:
        pass

    def repository_name(self) -> str:
        raise NotImplementedError

    def save(self, frame_id: str, frame: str) -> None:
        raise NotImplementedError

    def find(self, frame_id: str) -> str:
        raise NotImplementedError

    def update(self, frame_id: str, frame: str) -> str:
        raise NotImplementedError

    def delete(self, frame_id: str, frame: str) -> None:
        raise NotImplementedError
    
    def delete_all(self) -> None:
        raise NotImplementedError

    def get_data_size(self) -> int:
        raise NotImplementedError
    
# common behaviour:
    # delete_all
    # get_data_size
    # repository_name
    # __init__ (connect to database)
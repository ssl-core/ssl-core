from __future__ import annotations
from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from abc import ABC, abstractmethod

class RepositoryAbstractFactory(ABC):
    @abstractmethod
    def createFrameRepositoryKeyValue(self) -> IFrameRepositoryKeyValue:
        pass

    @abstractmethod
    def createFrameRepositoryDocument(self) -> IFrameRepositoryDocument:
        pass
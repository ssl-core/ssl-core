from __future__ import annotations
from src.data.Frame.IFrameRepository import IFrameRepository
from abc import ABC, abstractmethod

class RepositoryAbstractFactory(ABC):
    @abstractmethod
    def createFrameRepository(self) -> IFrameRepository:
        pass
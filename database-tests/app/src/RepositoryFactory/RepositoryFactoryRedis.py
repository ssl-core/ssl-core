from src.data.Frame.IFrameRepository import IFrameRepository
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryRedis.FrameRepositoryRedis import FrameRepositoryRedis

class RepositoryFactoryRedis(RepositoryAbstractFactory):
  def createFrameRepository(self) -> IFrameRepository:
    return FrameRepositoryRedis()
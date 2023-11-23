from src.data.Frame.IFrameRepository import IFrameRepository
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryCassandra.FrameRepositoryCassandra import FrameRepositoryCassandra

class RepositoryFactoryCassandra(RepositoryAbstractFactory):
  def createFrameRepository(self) -> IFrameRepository:
    return FrameRepositoryCassandra()
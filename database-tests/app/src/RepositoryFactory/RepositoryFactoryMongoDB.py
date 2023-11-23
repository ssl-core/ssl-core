from src.data.Frame.IFrameRepository import IFrameRepository
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryMongoDB.FrameRepositoryMongoDB import FrameRepositoryMongoDB

class RepositoryFactoryMongoDB(RepositoryAbstractFactory):
  def createFrameRepository(self) -> IFrameRepository:
    return FrameRepositoryMongoDB()
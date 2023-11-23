from src.data.Frame.IFrameRepository import IFrameRepository
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryScyllaDB.FrameRepositoryScyllaDB import FrameRepositoryScyllaDB

class RepositoryFactoryScyllaDB(RepositoryAbstractFactory):
  def createFrameRepository(self) -> IFrameRepository:
    return FrameRepositoryScyllaDB()
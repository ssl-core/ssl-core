from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryCassandra.FrameRepositoryCassandraKeyValue import FrameRepositoryCassandraKeyValue
from src.data.Frame.FrameRepositoryCassandra.FrameRepositoryCassandraDocument import FrameRepositoryCassandraDocument

class RepositoryFactoryCassandra(RepositoryAbstractFactory):
  def createFrameRepositoryKeyValue(self) -> IFrameRepositoryKeyValue:
    return FrameRepositoryCassandraKeyValue()
  
  def createFrameRepositoryDocument(self) -> IFrameRepositoryDocument:
    return FrameRepositoryCassandraDocument()